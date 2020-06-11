# Assign or Float Comparison in Branch Condition Checker
AssignOrFloatCompInBranchCondChecker is Clang Static Analyzer checker for assignment as controlling statement in branch condition and floating type comparison in loop condition.

## Setup
- Build [LLVM](https://github.com/llvm/llvm-project) as described in README
- Copy `AssignOrFloatCompInBranchCondChecker.cpp` to the `llvm-project/clang/lib/StaticAnalyzer/Checkers` folder
- Extend `llvm-project/clang/lib/StaticAnalyzer/Checkers/CMakeLists.txt` with `AssignOrFloatCompInBranchCondChecker.cpp`
- Add following block of code into `Checkers.td` file contained in `llvm-project/clang/include/clang/StaticAnalyzer/Checkers`: 
```
let ParentPackage = UnixAlpha in {
    ...
    def AssignOrFloatCompInBranchCondChecker : Checker<"AssignOrFloatCompInBranchCondChecker">,  
      HelpText<"Check for assignment or float type comparison in condition statement">,  
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
