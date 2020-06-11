#include "clang/AST/ParentMap.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include <utility>

using namespace clang;
using namespace ento;

namespace {

class AssignOrFloatCompInBranchCondChecker
    : public Checker<check::BranchCondition> {
  mutable std::unique_ptr<BuiltinBug> assignBT;
  mutable std::unique_ptr<BuiltinBug> floatCntBT;

  void ReportBug(CheckerContext &Ctx, SourceRange range,
                 std::unique_ptr<BuiltinBug> &BT) const;
  bool isRightCommaOperandAssignment(const Stmt *Statement) const;
  bool isLoopCounterFloat(const Stmt *Statement) const;

public:
  void checkBranchCondition(const Stmt *Condition, CheckerContext &Ctx) const;
};

} // namespace

void AssignOrFloatCompInBranchCondChecker::ReportBug(
    CheckerContext &Ctx, SourceRange range,
    std::unique_ptr<BuiltinBug> &BT) const {

  ExplodedNode *N = Ctx.generateErrorNode();
  if (!N)
    return;

  auto R =
      std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);
  R->addRange(range);
  Ctx.emitReport(std::move(R));
}

// checks if right operand of comma operator in condition statement is
// assignment, W-parenthesis rule already checks if ONLY expression in condition
// statement is assignment

bool AssignOrFloatCompInBranchCondChecker::isRightCommaOperandAssignment(
    const Stmt *Statement) const {

  if (const BinaryOperator *BinOperator = dyn_cast<BinaryOperator>(Statement)) {

    if (BinOperator->isCommaOp()) {

      const Expr *rightEx = BinOperator->getRHS();
      const BinaryOperator *BO = dyn_cast<BinaryOperator>(rightEx);

      if (BO) {
        if (BO->isAssignmentOp())
          return true;
      }
    }
  }
  return false;
}

// checks if loop counter is a floating point variable

bool AssignOrFloatCompInBranchCondChecker::isLoopCounterFloat(
    const Stmt *Statement) const {

  if (const BinaryOperator *BinOperator = dyn_cast<BinaryOperator>(Statement)) {
    BinaryOperator::Opcode Op = BinOperator->getOpcode();

    if (BinaryOperator::isComparisonOp(Op)) {

      // only check floating point comparison if operator is (==), (<=) or (>=);
      // (<) and (>) is ok
      if (Op == BO_GT || Op == BO_LT)
        return false;

      const Expr *rightEx = BinOperator->getRHS();
      const Expr *leftEx = BinOperator->getLHS();
      QualType leftType = leftEx->getType();
      QualType rightType = rightEx->getType();

      if (leftType->isFloatingType() && rightType->isFloatingType())
        return true;
    }
  }
  return false;
}

void AssignOrFloatCompInBranchCondChecker::checkBranchCondition(
    const Stmt *Condition, CheckerContext &Ctx) const {

  // get original branch statement where condition is used, check if it is a
  // loop

  const ParentMap &parentMap = Ctx.getLocationContext()->getParentMap();
  if (const Stmt *branchStatement = parentMap.getParent(Condition)) {

    if (isa<ForStmt>(branchStatement) || isa<WhileStmt>(branchStatement) ||
        isa<DoStmt>(branchStatement)) {

      // check for loop counter float

      if (isLoopCounterFloat(Condition)) {

        if (!floatCntBT)
          floatCntBT.reset(
              new BuiltinBug(this, "Loop counter is float, can cause undefined "
                                   "behavior due to imprecise comparison"));
        const Expr *Ex = cast<Expr>(Condition);
        ReportBug(Ctx, Ex->getSourceRange(), floatCntBT);
      }
    }
  }

  // check for assignment

  if (isRightCommaOperandAssignment(Condition)) {

    if (!assignBT)
      assignBT.reset(new BuiltinBug(
          this, "Controlling operand in condition statement is assignment"));
    const Expr *Ex = cast<Expr>(Condition);
    ReportBug(Ctx, Ex->getSourceRange(), assignBT);
  }
}

void ento::registerAssignOrFloatCompInBranchCondChecker(CheckerManager &mgr) {
  mgr.registerChecker<AssignOrFloatCompInBranchCondChecker>();
}

bool ento::shouldRegisterAssignOrFloatCompInBranchCondChecker(
    const CheckerManager &mgr) {
  return true;
}
