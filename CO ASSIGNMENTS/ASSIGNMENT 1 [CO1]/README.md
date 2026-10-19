# Assignment 1 [CO1] — Metro Smart-Card Fare System

Student: Hitesh  
Register number: 192524005

This folder contains the written answers, C++ programs, run transcripts, and evidence for Tasks 01–12. Start with `REPORT_DRAFT.md`; each task's detailed response is in its `answer.md` file.

## Task map and short results

| Task | Files | Result |
|---|---|---|
| 01 — Objects and OOP | `Task01/answer.md`, `Task01/evidence/uml_class_diagram.svg` | Explains the objects, invariants, OOP principles, and procedural alternative; includes the rendered UML asset and editable Mermaid source. |
| 02 — Tokens and constants | `Task02/answer.md` | Identifies token kinds and recommends `constexpr` and `enum class` for the fare rules. |
| 03 — Type choices | `Task03/answer.md`, `types.cpp`, `RUN_OUTPUT.txt` | Measures sizes/ranges including `unsigned` and `auto`; shows why a 32-bit `int` cannot hold the largest network total in paise. |
| 04 — Conversions | `Task04/answer.md`, `conversions.cpp`, `MY_PREDICTION.txt`, `RUN_OUTPUT.txt` | Output matches the reference prediction; the signed/unsigned warning is intentional and explained. |
| 05 — Operators | `Task05/answer.md`, `operators.cpp`, `MY_PREDICTION.txt`, `RUN_OUTPUT.txt` | Output matches the reference prediction; precedence, increment, division, and remainder are explained. |
| 06 — Control structures | `Task06/answer.md`, `pseudocode.txt`, `fare_complete.cpp`, `RUN_OUTPUT.txt` | Runs the sample journeys and verifies that exactly 10:00 and 20:00 are off-peak. |
| 07 — Procedural vs OOP | `Task07/answer.md`, both `.cpp` files and output transcripts | Both versions approve the trip and leave 6400 paise. |
| 08 — Safe input/output | `Task08/answer.md`, `safe_io.cpp`, input files and output transcripts | Prints a journey report; valid input is approved, while malformed and out-of-range input is rejected and retried. |
| 09 — Testing | `Task09/answer.md`, `fare_tests.cpp`, `test_plan.csv`, `RUN_OUTPUT.txt` | The recorded test run reports zero failures. |
| 10 — Scale and precision | `Task10/answer.md`, `scale.cpp`, `RUN_OUTPUT.txt` | Compares exact integer-paise totals with floating-point accumulation at three trip counts. |
| 11 — Recommendation | `Task11/answer.md` | Recommends the design using the earlier tests and scale results. |
| 12 — Design challenge | `Task12/answer.md`, `redesigned_fare.cpp`, `RUN_OUTPUT.txt` | Demonstrates Airport fares, weekend discount, and pass coverage. |

## Build and run each program

Use an LLVM-MinGW terminal with `g++` available. Run each command from the named task folder. The commands use C++11 and enable common warnings.

```powershell
# Task03
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic types.cpp -o types.exe
.\types.exe

# Task04 (one signed/unsigned comparison warning is expected)
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic conversions.cpp -o conversions.exe
.\conversions.exe

# Task05
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic operators.cpp -o operators.exe
.\operators.exe

# Task06
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic fare_complete.cpp -o fare_complete.exe
.\fare_complete.exe

# Task07 — compile and run each file separately
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic procedural.cpp -o procedural.exe
.\procedural.exe
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic object_oriented.cpp -o object_oriented.exe
.\object_oriented.exe

# Task08 — replay the recorded valid and recovery inputs
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic safe_io.cpp -o safe_io.exe
Get-Content valid_input.txt | .\safe_io.exe
Get-Content invalid_input.txt | .\safe_io.exe

# Task09
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic fare_tests.cpp -o fare_tests.exe
.\fare_tests.exe

# Task10
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic scale.cpp -o scale.exe
.\scale.exe

# Task12
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic redesigned_fare.cpp -o redesigned_fare.exe
.\redesigned_fare.exe
```

Tasks01, 02, and 11 are written analysis/design tasks and do not have standalone programs. Actual run records are in each coding task's `RUN_OUTPUT.txt` or named output transcript. Every task has an `evidence/` folder containing screenshots of its answer/source and, where applicable, its run transcript. Output screenshots show the saved run transcript open in Notepad; they are not screenshots of a live terminal.

## Notes before submission

- Task04 and Task05 prediction files are clearly labelled reference predictions prepared with assistance. If the instructor requires each student to predict independently before running, write your own predictions and then rerun the programs; do not submit the reference prediction as independent work.
- The assignment does not specify the student/senior concession rate. Any 50% example in the design discussion is explicitly an assumption; confirm the actual rate and stacking order with the instructor.
- The PDF requests a repository named `DSA01-<RegisterNumber>-Assignment1`. This work is in the subject repository `DSA0101-192524005` because that repository was selected for the assignment. Ask the instructor whether that naming difference is acceptable.
- The report uses the supplied account name `Hitesh`; replace it with the full name your instructor expects if necessary.
