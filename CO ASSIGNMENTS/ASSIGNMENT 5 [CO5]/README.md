# Assignment 5 — Reliable C++ Billing and Claims

**Student:** Hitesh C  
**Register number:** 192524005  
**Course:** C++ Programming · **Course outcome:** CO5  
**Repository:** [DSA0101-192524005](https://github.com/Hitesh-92007/DSA0101-192524005/tree/main/CO%20ASSIGNMENTS)

This folder contains the CO5 design answers, completed programs, validation cases, run records, benchmark data, evidence views and final report PDF. The task folders follow Parts A and B of the supplied Assignment 5 PDF.

## Compiler and commands

The available `g++` command is the LLVM-MinGW toolchain; `g++ --version` reports **clang version 23.1.1**. The programs use C++17.

Compile the billing program from this folder:

```powershell
g++ -std=c++17 -Wall -Wextra -ICommon Task_B1/billing_complete.cpp -o billing.exe
./billing.exe
```

Run the completed sample, exception tests, extension, and benchmark:

```powershell
g++ -std=c++17 -Wall -Wextra Task_B1/complete_code.cpp -o complete_code.exe
./complete_code.exe

g++ -std=c++17 -Wall -Wextra -ICommon Task_B2/exception_tests.cpp -o exception_tests.exe
./exception_tests.exe

g++ -std=c++17 -Wall -Wextra -ICommon Task_B4/production_extension.cpp -o extension.exe
./extension.exe

g++ -std=c++17 -O2 -Wall -Wextra Task_B3/benchmark.cpp -o benchmark.exe
./benchmark.exe
```

AddressSanitizer runs:

```powershell
g++ -std=c++17 -Wall -Wextra -fsanitize=address -ICommon -DCO5_ASAN_SAFE Task_B2/exception_tests.cpp -o exception_tests_asan.exe
$env:ASAN_OPTIONS='detect_leaks=0'
./exception_tests_asan.exe

g++ -std=c++17 -Wall -Wextra -fsanitize=address -ICommon Task_B4/production_extension.cpp -o extension_asan.exe
./extension_asan.exe
```

This Windows AddressSanitizer runtime reports that leak detection is unsupported. `detect_leaks=0` lets AddressSanitizer check the exception runs for other supported memory errors. The sanitizer version of the allocation test injects `std::bad_alloc`; the non-sanitized run makes the actual impossible-size allocation request. The deliberately non-virtual destructor example is undefined behavior and is kept only as a controlled demonstration.

## Task map

| Folder | Contents / result |
|---|---|
| `Task_A1` | Abstract bill hierarchy, UML, processing pseudocode and switch alternative |
| `Task_A2` | Early/late binding, vtables, virtual destructor and measured example |
| `Task_A3` | Abstract payer interface and extension design |
| `Task_A4` | Error map, exception hierarchy and `noexcept` decisions |
| `Task_A5` | Stack unwinding, recovery, exact prediction and rethrow output |
| `Task_B1` | Completed TODO program, all bill/payer checks and compile-error experiment |
| `Task_B2` | Invalid inputs, allocation recovery, output prediction and claim rethrow program |
| `Task_B3` | 10-bill correctness, 1K/1M/10M timing and 1M-claim error-mode comparison |
| `Task_B4` | New Telemedicine bill and CorporateTieUp payer, exception and ASan run |
| `Common` | Shared billing and payer interfaces used by the implementation tasks |
| `Report` | Complete CO5 assignment report PDF |

## Evidence note

The `.txt` run records are saved program output. PNGs in the evidence folders are clearly captioned rendered views of the source or saved console transcript; they are not presented as live desktop screenshots. AddressSanitizer results and platform limits are recorded in the report.
