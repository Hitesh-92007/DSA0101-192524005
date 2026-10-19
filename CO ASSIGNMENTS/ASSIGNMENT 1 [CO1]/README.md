# Assignment 1 [CO1] — Metro Smart-Card Fare System

This folder contains the task answers, C++ programs, actual console run records, and per-task screenshot evidence. Open `Task01/answer.md` for the object analysis and UML sketch. Each task folder has its own `answer.md`, `EVIDENCE_TO_ADD.txt`, and `evidence/` folder describing or showing its evidence.

## Task map

| Task | Answer | Code / run evidence |
|---|---|---|
| 01 Objects and OOP | `Task01/answer.md` | Analysis and Mermaid UML; no executable required |
| 02 Tokens and constants | `Task02/answer.md` | Constant/code examples in the answer |
| 03 Types | `Task03/answer.md` | `Task03/types.cpp`, `Task03/RUN_OUTPUT.txt` |
| 04 Conversions | `Task04/answer.md` | `Task04/conversions.cpp`, prediction, `Task04/RUN_OUTPUT.txt` |
| 05 Operators | `Task05/answer.md` | `Task05/operators.cpp`, prediction, `Task05/RUN_OUTPUT.txt` |
| 06 Control structures | `Task06/answer.md` | Pseudocode, `Task06/fare_complete.cpp`, `Task06/RUN_OUTPUT.txt` |
| 07 Procedural vs OOP | `Task07/answer.md` | Both `.cpp` files and both output transcripts |
| 08 Safe I/O | `Task08/answer.md` | `safe_io.cpp`, valid and invalid input transcripts |
| 09 Testing | `Task09/answer.md` | Test table, `fare_tests.cpp`, `test_plan.csv`, run transcript |
| 10 Scale and precision | `Task10/answer.md` | `scale.cpp` and measured output transcript |
| 11 Recommendation | `Task11/answer.md` | Recommendation supported by Tasks09, 10, and 12 |
| 12 Design challenge | `Task12/answer.md` | `redesigned_fare.cpp` and run transcript |

## Build and evidence status

- Compiler used: `g++` command supplied by LLVM-MinGW, reporting Clang 23.1.1; C++11.
- Compile command: `g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic source.cpp -o program.exe`.
- Tasks03–10 and 12 were compiled and run; their actual console transcripts are saved in their task folders. Task04 has the expected signed/unsigned comparison warning. See `BUILD_RESULTS.md`.
- Each task's `evidence/` folder now contains screenshots of the relevant source/answer and, for coding tasks, the saved actual run transcript. Output images capture the transcript displayed in Notepad; they are screenshots of the recorded output text, not live terminal windows. The original `.txt` transcripts remain available. Tasks04/05 predictions are reference predictions prepared with assistance; replace them with your own if your instructor requires independent predictions.
- Add your name and register number to `REPORT_DRAFT.md`. The scenario does not specify the student/senior concession percentage, so the report marks its 50% example as an assumption to confirm.
- The PDF specifies a repository named `DSA01-<RegisterNumber>-Assignment1`; the selected repository is `DSA0101-192524005`. Confirm with the instructor that this repository name is acceptable.
