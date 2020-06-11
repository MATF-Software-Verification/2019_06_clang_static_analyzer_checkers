# Math function arguments checker
MathFuncParamChecker is Clang Static Analyzer checker for input arguments of math functions (currently for sqrt and pow), but could be generalized for custom function.

## Setup
- Build [LLVM](https://github.com/llvm/llvm-project) as described in README
- Copy `MathFuncParamChecker.cpp` to the `llvm-project/clang/lib/StaticAnalyzer/Checkers` folder
- Extend `llvm-project/clang/lib/StaticAnalyzer/Checkers/CMakeLists.txt` with `MathFuncParamChecker.cpp`
- Add following block of code into `Checkers.td` file contained in `llvm-project/clang/include/clang/StaticAnalyzer/Checkers`: 
```
let ParentPackage = UnixAlpha in {
    ...
    def MathFuncParamChecker : Checker<"MathFuncParamChecker">,  
      HelpText<"Check for math functions arguments">,  
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
