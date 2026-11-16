# Assignment 4 CO4 Designing Reusable Class Hierarchies

**Course:** Object Oriented Programming with C++ (DSA01)  
**Student:** Hitesh C  
**Register number:** 192524005  
**Nature:** Individual assignment  
**Repository:** https://github.com/Hitesh-92007/DSA0101-192524005/tree/main/CO%20ASSIGNMENTS/ASSIGNMENT%204%20%5BCO4%5D

This folder contains answers, C++17 programs, test inputs, run records, output captures and the final report for all six tasks in Assignment 4.

## Compiler and build commands

The compiler available for these runs is the `g++` driver supplied by LLVM-MinGW. Its version output identifies Clang 23.1.1. The code was compiled with C++17 and `-Wall -Wextra`. To use the short `g++` command below, add the LLVM-MinGW `bin` folder to `PATH` first.

```powershell
g++ --version
g++ -std=c++17 -Wall -Wextra Task01/payroll_hierarchy.cpp -o Task01/payroll_hierarchy.exe
```

Use the build script to compile and run the full set from this folder:

```powershell
.\BUILD_AND_RUN.ps1
```

To compile and run one program, replace the source and executable paths with the paths shown in that task's `RUN_OUTPUT.txt`:

```powershell
g++ -std=c++17 -Wall -Wextra Task01/payroll_hierarchy.cpp -o Task01/payroll_hierarchy.exe
.\Task01\payroll_hierarchy.exe
```

`BUILD_AND_RUN.ps1` contains the compile and run commands for all 17 runnable programs. `Task02/person_access_ambiguity.cpp` is a separate expected compile failure that demonstrates the ambiguous `name` member; its diagnostic is saved in `Task02/EXPECTED_DIAGNOSTIC.txt`. Each task folder contains an `answer.md`, sample input, source code, and saved run output. See `BUILD_RESULTS.md` for the status of every compile and run.

## Task guide

| Folder | Contents |
|---|---|
| `Task01` | Payroll design review, repaired hierarchy, UML, and alternative design |
| `Task02` | People hierarchy, virtual diamond, composition, access table, and prediction |
| `Task03` | Initializer lists, member classes, construction trace, and complete-code program |
| `Task04` | Account defects, AddressSanitizer evidence, repaired polymorphism, and slicing |
| `Task05` | Cart object access, pointers, references, `this`, chaining, and highest-total search |
| `Task06` | Vehicle-rental hierarchy, pseudocode, rent/return demo, and ElectricBus extension |
| `Report` | Complete report in Word, PDF, and Markdown |

The assignment requires each student to run every task and write predictions before running. This folder was prepared with coding assistance; review the code, replace the reference predictions with your own before your local runs, and verify the results yourself. The included output images are formatted from saved run logs, not screenshots of a live terminal. Add genuine screenshots of your local source and execution as required by the assignment before submitting. The PDF leaves the submission date blank.
