# SimpleAllocationOperatorsChecker
Simple checker that triggers some basic sanity checks when using <i> new </i> and <i> delete </i> operators.

Functionalities:
  - Show warning when <i> delete </i> is called on a nullptr
  - Show warning when <i> delete </i> is called multiple times on the same memory location
  - Show warning when fields or methods are accessed through an instance that has been deallocated with <i> delete </i> operator
  - Show warning when objects were instantiated with <i> new </i> operator without calling delete afterwards at some point
  - Show warning when there is possibility of calling delete on nullptr (e.g. when nullability isn't checked with if statement)

## Setup
- Build [LLVM](https://github.com/llvm/llvm-project) as described in README
- Copy `DeleteOperatorChecker.cpp` to the `llvm-project/clang/lib/StaticAnalyzer/Checkers` folder
- Extend `llvm-project/clang/lib/StaticAnalyzer/Checkers/CMakeLists.txt` with `DeleteOperatorChecker.cpp`
- Add following block of code into `Checkers.td` file contained in `llvm-project/clang/include/clang/StaticAnalyzer/Checkers`: 
```
let ParentPackage = UnixAlpha in {
    ...
    def MathFuncParamChecker : Checker<"DeleteOperatorChecker">,  
      HelpText<"Does some allocation/deallocation basic error checks.">,  
      Documentation<NotDocumented>;
    ...
} // end "alpha.unix" 
```
- Rebuild project

## Usage
For applications written in C, run:

```
clang --analyze -Xanalyzer -analyzer-checker=unix,alpha example.c
```
For applications written in C++, run:
```
clang++ --analyze -Xanalyzer -analyzer-checker=unix,alpha example.cpp
```
Both `clang` and `clang++` can be found in `llvm-project/build/bin`.
