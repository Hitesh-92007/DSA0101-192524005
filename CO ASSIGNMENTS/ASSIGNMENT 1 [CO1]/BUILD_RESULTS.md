# Build and run results

Compiler invoked as `g++`; this Windows installation reports LLVM-MinGW / Clang 23.1.1. Target: x86_64 Windows GNU. Standard: C++11. The commands used were:

```text
g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic source.cpp -o program.exe
.\program.exe
```

## Compilation and execution

| Task | Source | Build | Run record |
|---|---|---|---|
| 03 | `Task03/types.cpp` | Success, no diagnostics | `Task03/RUN_OUTPUT.txt`, exit 0 |
| 04 | `Task04/conversions.cpp` | Success; one intentional signed/unsigned comparison warning | `Task04/RUN_OUTPUT.txt`, exit 0 |
| 05 | `Task05/operators.cpp` | Success, no diagnostics | `Task05/RUN_OUTPUT.txt`, exit 0 |
| 06 | `Task06/fare_complete.cpp` | Success, no diagnostics | `Task06/RUN_OUTPUT.txt`, exit 0 |
| 07 | `Task07/procedural.cpp`, `Task07/object_oriented.cpp` | Both success, no diagnostics | Both output files, exit 0 |
| 08 | `Task08/safe_io.cpp` | Success, no diagnostics | Valid input exit 0; invalid input rejected with expected exit 1 |
| 09 | `Task09/fare_tests.cpp` | Success, no diagnostics | `Task09/RUN_OUTPUT.txt`, 0 failures, exit 0 |
| 10 | `Task10/scale.cpp` | Success, no diagnostics | `Task10/RUN_OUTPUT.txt`, exit 0 |
| 12 | `Task12/redesigned_fare.cpp` | Success, no diagnostics | `Task12/RUN_OUTPUT.txt`, exit 0 |

Task04 and Task05 predictions were recorded with assistance before their runs; see the task folders. Task09 includes the requested nonnumeric-hour parser check. Task08 includes both valid input and recovery/rejection transcripts.

These `.txt` files are actual console transcripts from the executions above. They are not screenshots. The assignment's general submission requirements also ask for output screenshots; genuine screenshots still need to be added before final submission. Tasks01 and 02 are written analysis/design tasks rather than separate executable programs.
