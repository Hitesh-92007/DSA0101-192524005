# Build and run results

Compiler: `g++ (tdm64-1) 4.9.2` (Dev-C++)  
Standard: C++11  
Flags: `-Wall -Wextra -Wconversion -pedantic`

## Compilation

All ten C++ source files compile successfully:

| Task | Source | Result |
|---|---|---|
| 03 | `types.cpp` | Pass |
| 04 | `conversions.cpp` | Pass; expected signed/unsigned comparison warning for the conversion example |
| 05 | `operators.cpp` | Pass |
| 06 | `fare_complete.cpp` | Pass |
| 07 | `procedural.cpp`, `object_oriented.cpp` | Pass |
| 08 | `safe_io.cpp` | Pass |
| 09 | `fare_tests.cpp` | Pass |
| 10 | `scale.cpp` | Pass |
| 12 | `redesigned_fare.cpp` | Pass |

## Executed programs

| Task | Run result | Saved output |
|---|---|---|
| 03 | Exit 0 | `Task03/RUN_OUTPUT.txt` |
| 04 | Not run yet; add and date the required personal prediction first | — |
| 05 | Not run yet; add and date the required personal prediction first | — |
| 06 | Exit 0 | `Task06/RUN_OUTPUT.txt` |
| 07 procedural | Exit 0; leaves 6400 paise | `Task07/procedural_output.txt` |
| 07 object-oriented | Exit 0; leaves 6400 paise | `Task07/object_oriented_output.txt` |
| 08 valid input | Exit 0 | `Task08/valid_input_output.txt` |
| 08 invalid input | Exit 1 as expected for negative recharge; nonnumeric input was rejected and retried | `Task08/invalid_input_output.txt` |
| 09 | Exit 0; all cases pass, failures = 0 | `Task09/RUN_OUTPUT.txt` |
| 10 | Exit 0 | `Task10/RUN_OUTPUT.txt` |
| 12 | Exit 0 | `Task12/RUN_OUTPUT.txt` |

The Task07 programs now save the result of the deduction before printing the balance, avoiding an evaluation-order ambiguity in C++11. Task12 likewise stores the payment result before printing the updated balance.

The `.txt` files are console transcripts from these local runs. Add any screenshots the assignment PDF requests; transcripts do not replace screenshots.

