# Assignment 2 [CO2] - C++ Functions and Object-Oriented Concepts

**Student:** Hitesh  
**Register number:** 192524005  
**Compiler:** `g++` driver from LLVM-MinGW, Clang 23.1.1, target `x86_64-w64-windows-gnu`  
**Language standard:** C++17

This folder contains the seven assignment tasks, C++ source, answers, compiler output, and program run records. The full report is available as [`Report/Report.pdf`](Report/Report.pdf) and [`Report/Report.md`](Report/Report.md). Each source file is in its task folder.

## Repository note

The assignment PDF asks for a new repository named `DSA01-<RegisterNumber>-Assignment2`. This work was placed in the existing `DSA0101-192524005` repository because that is the repository selected for this task. Please confirm that this naming difference is accepted by the course faculty.

## Compile and run

Run commands from the task folder in a PowerShell terminal where `g++` is available. These are the compile flags used for the normal task programs:

```powershell
g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q1_call_modes.cpp -o q1_call_modes.exe
.\q1_call_modes.exe

g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q2_return_reference.cpp -o q2_return_reference.exe
.\q2_return_reference.exe

g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q3_courier_overloads.cpp -o q3_courier_overloads.exe
.\q3_courier_overloads.exe

g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q4_savings_account.cpp -o q4_savings_account.exe
.\q4_savings_account.exe

g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q5_static_bus_tickets.cpp -o q5_static_bus_tickets.exe
.\q5_static_bus_tickets.exe

g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q7_cached_highest.cpp -o q7_cached_highest.exe
.\q7_cached_highest.exe
```

Q2 intentionally includes an unused function that returns a reference to a local variable so the compiler warning can be shown. Do **not** call that function. The normal compile produces the warning in `Task02/compiler_warning.txt`.

For Q6, compile all four variants and run each. Replace `q6_friend_inline.cpp` with the source filename in the commands below:

```powershell
g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O0 q6_friend_inline.cpp -o q6_noinline_O0.exe
g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O0 -DUSE_INLINE q6_friend_inline.cpp -o q6_inline_O0.exe
g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q6_friend_inline.cpp -o q6_noinline_O2.exe
g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 -DUSE_INLINE q6_friend_inline.cpp -o q6_inline_O2.exe
.\q6_noinline_O0.exe
.\q6_inline_O0.exe
.\q6_noinline_O2.exe
.\q6_inline_O2.exe
```

Q6 makes five runs per variant in the saved evidence. Q7 creates its test marks internally; no separate input file is required. Q1-Q5 use fixed examples in `main()` and do not require input files.

## Task map and results

| Task | Source and answer | Result |
|---|---|---|
| Q1 | `Task01/q1_call_modes.cpp`, `Task01/answer.md` | Value leaves the caller's fee unchanged; reference and pointer update it. The given program prints `1 12 13`, `1 100`, and `16 13 6`. |
| Q2 | `Task02/q2_return_reference.cpp`, `Task02/answer.md` | The reference updates the highest mark; the intentionally unsafe local-reference function emits a compiler warning and is never called. |
| Q3 | `Task03/q3_courier_overloads.cpp`, `Task03/answer.md` | Six non-ambiguous calls select the expected integer, double, or `Parcel` overload. |
| Q4 | `Task04/q4_savings_account.cpp`, `Task04/answer.md` | The circular array prints the latest ten of twelve transactions; balance ends at 1,560. |
| Q5 | `Task05/q5_static_bus_tickets.cpp`, `Task05/answer.md` | Token IDs are 101-104; three seats are issued and later tickets wait. |
| Q6 | `Task06/q6_friend_inline.cpp`, `Task06/answer.md` | Time checks pass; each benchmark runs 10 million calls with both `-O0` and `-O2`, with and without the `inline` keyword. |
| Q7 | `Task07/q7_cached_highest.cpp`, `Task07/answer.md` | Scan and cache return the same maximum and checksum; the cached design handles a lowered maximum by rescanning. |

Each task folder has its `.cpp` source, `answer.md`, `RUN_OUTPUT.txt`, and `evidence/source_view.png` plus `evidence/output_view.png`. Q6's five-run measurements are in `Task06/q6_*_runs.txt`; the compiler warning for Q2 is in `Task02/compiler_warning.txt`. The report includes the task analyses and both captures for every task.

## Evidence and integrity

The text run records are from executions in the Codex workspace using the compiler listed above. The assignment requires the student to predict before running and to execute each task individually. The predictions in the report are reference answers, not evidence that the student made a prediction before a run. Review the code, write your own predictions before rerunning, and capture your own source/output screenshots for submission if your instructor requires evidence from your lab environment.

This assignment was prepared with AI coding and writing assistance. Hitesh should review every explanation and be ready to explain the programs during the viva. No personal execution or independent authorship is claimed here.

