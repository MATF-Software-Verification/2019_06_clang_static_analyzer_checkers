//=- MathFuncParamChecker.cpp --------------------------*- C++ -*-==//
// This checker implements rule 6.2 FLP32-C by "SEI CERT C Coding Standard":
// Prevent or detect domain and range errors in math functions
//===----------------------------------------------------------------------===//
//
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include <iostream>
#include <utility>

using namespace clang;
using namespace ento;

class MathFuncParamChecker : public Checker<check::PreStmt<CallExpr>> {
  mutable std::unique_ptr<BugType> PowBugtype;
  mutable std::unique_ptr<BugType> SqrtBugType;
  void ReportBug(CheckerContext &C, ProgramStateRef State, SourceRange SR,
                 std::unique_ptr<BugType> &BT) const;
  void checkPowArgs(const CallExpr *CE, CheckerContext &C) const;
  void checkSqrtArgs(const CallExpr *CE, CheckerContext &C) const;

public:
  MathFuncParamChecker();
  void checkPreStmt(const CallExpr *CE, CheckerContext &C) const;
};

MathFuncParamChecker::MathFuncParamChecker() {
  PowBugtype.reset(new BugType(this, "Domain/Range/Poll pow error",
                               "Math func input args error"));
  SqrtBugType.reset(new BugType(this, "Domain/Range/Poll sqrt error",
                                "Math func input args error"));
}

void MathFuncParamChecker::ReportBug(CheckerContext &C, ProgramStateRef State,
                                     SourceRange SR,
                                     std::unique_ptr<BugType> &BT) const {
  ExplodedNode *N = C.generateErrorNode(State);
  if (!N)
    return;

  auto Report =
      std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);
  Report->addRange(SR);
  C.emitReport(std::move(Report));
}
void MathFuncParamChecker::checkPreStmt(const CallExpr *CE,
                                        CheckerContext &C) const {
  const FunctionDecl *FD = C.getCalleeDecl(CE);
  if (!FD || FD->getKind() != Decl::Function)
    return;

  const DeclContext *NamespaceCtx = FD->getEnclosingNamespaceContext();
  if (NamespaceCtx && isa<NamespaceDecl>(NamespaceCtx))
    return;

  StringRef FName = C.getCalleeName(FD);
  if (FName.empty())
    return;

  if (FName == "pow")
    checkPowArgs(CE, C);
  else if (FName == "sqrt")
    checkSqrtArgs(CE, C);
  else
    return;
}

void MathFuncParamChecker::checkSqrtArgs(const CallExpr *CE,
                                         CheckerContext &C) const {
  const Expr *InputArgX = CE->getArg(0);
  if (!InputArgX)
    return;
  ProgramStateRef state = C.getState();

  QualType TypeArgX = InputArgX->getType();

  if (TypeArgX->isArithmeticType() && C.isGreaterOrEqual(InputArgX, 0)) {
    return;
  }

  if (TypeArgX->isArithmeticType() && C.isNegative(InputArgX)) {
    SqrtBugType.reset(new BugType(this,
                                  "Function argument is negative, domain error",
                                  "Math func input args error"));
    ReportBug(C, state, InputArgX->getSourceRange(), SqrtBugType);
    return;
  }

  const LocationContext *LC = C.getLocationContext();
  SVal ArgXSVal = C.getState()->getSVal(InputArgX, LC);

  Optional<DefinedSVal> DV = ArgXSVal.getAs<DefinedSVal>();
  if (!DV)
    return;
  ConstraintManager &CM = C.getConstraintManager();
  ProgramStateRef StateTrue, StateFalse;

  std::tie(StateTrue, StateFalse) = CM.assumeDual(C.getState(), *DV);

  if (StateTrue && StateFalse) {
    SqrtBugType.reset(
        new BugType(this, "Undefined function argument, could be negative",
                    "Math func input args error"));
    ReportBug(C, state, InputArgX->getSourceRange(), SqrtBugType);
    return;
  }
}

void MathFuncParamChecker::checkPowArgs(const CallExpr *CE,
                                        CheckerContext &C) const {
  const Expr *InputArgX = CE->getArg(0);
  if (!InputArgX)
    return;
  const Expr *InputArgY = CE->getArg(1);
  if (!InputArgY)
    return;
  ProgramStateRef state = C.getState();

  QualType TypeArgX = InputArgX->getType();
  QualType TypeArgY = InputArgY->getType();

  if (!TypeArgX->isArithmeticType() || !TypeArgY->isArithmeticType())
    return;

  if (C.isGreaterOrEqual(InputArgX, 1))
    return;

  if (C.isNegative(InputArgY) && !C.isNegative(InputArgX) &&
      !C.isGreaterOrEqual(InputArgX, 1)) {
    PowBugtype.reset(new BugType(
        this,
        "Pole error: if first argument is 0, second argument must be positive",
        "Math func input args error"));
    ReportBug(C, state, InputArgX->getSourceRange(), PowBugtype);
    return;
  }

  if (C.isNegative(InputArgX) && !TypeArgY->isIntegerType()) {
    PowBugtype.reset(new BugType(this,
                                 "Domain error: if first argument is less than "
                                 "0, second argument must be an integer",
                                 "Math func input args error"));
    ReportBug(C, state, InputArgX->getSourceRange(), PowBugtype);
    return;
  }

  const LocationContext *LC = C.getLocationContext();
  SVal ArgXSVal = C.getState()->getSVal(InputArgX, LC);

  Optional<DefinedSVal> DVX = ArgXSVal.getAs<DefinedSVal>();
  if (!DVX)
    return;
  ConstraintManager &CM = C.getConstraintManager();
  ProgramStateRef StateTrue, StateFalse;

  std::tie(StateTrue, StateFalse) = CM.assumeDual(C.getState(), *DVX);

  if (StateTrue && StateFalse) {
    PowBugtype.reset(new BugType(
        this,
        "First pow argument is undefined, could cause domain or range error",
        "Math func input args error"));
    ReportBug(C, state, InputArgX->getSourceRange(), PowBugtype);
    return;
  }

  SVal ArgYSVal = C.getState()->getSVal(InputArgY, LC);
  Optional<DefinedSVal> DVY = ArgYSVal.getAs<DefinedSVal>();
  if (!DVY)
    return;

  std::tie(StateTrue, StateFalse) = CM.assumeDual(C.getState(), *DVY);
  if (StateTrue && StateFalse) {
    PowBugtype.reset(new BugType(
        this,
        "Second pow argument is undefined, could cause domain or range error",
        "Math func input args error"));
    ReportBug(C, state, InputArgX->getSourceRange(), PowBugtype);
    return;
  }
}

void ento::registerMathFuncParamChecker(CheckerManager &mgr) {
  mgr.registerChecker<MathFuncParamChecker>();
}

// This checker should be enabled regardless of how language options are set.
bool ento::shouldRegisterMathFuncParamChecker(const CheckerManager &mgr) {
  return true;
}
