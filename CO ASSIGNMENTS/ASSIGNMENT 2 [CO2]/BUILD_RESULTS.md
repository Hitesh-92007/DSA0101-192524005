# Build and run results

Compiler driver: LLVM-MinGW `g++`, Clang 23.1.1, target `x86_64-w64-windows-gnu`. Standard: C++17. Normal flags: `-std=c++17 -Wall -Wextra -Wconversion -pedantic -O2`.

| Task | Build result | Run evidence |
|---|---|---|
| Q1 | Success | `Task01/RUN_OUTPUT.txt` |
| Q2 | Success with the intended warning for returning a local reference; warning is not suppressed and the function is never called | `Task02/compiler_warning.txt`, `Task02/RUN_OUTPUT.txt` |
| Q3 | Success, no diagnostics | `Task03/RUN_OUTPUT.txt` |
| Q4 | Success, no diagnostics | `Task04/RUN_OUTPUT.txt` |
| Q5 | Success, no diagnostics | `Task05/RUN_OUTPUT.txt` |
| Q6 | All four configurations built and run five times; identical checksum in all runs | `Task06/q6_*_runs.txt` |
| Q7 | Success, no diagnostics | `Task07/RUN_OUTPUT.txt` |

Output records are text transcripts captured from program executions. Timings in Q6 and Q7 are machine-specific and can vary. The course handout requires student-generated pre-run predictions and individual execution; the saved reference results do not claim to prove that process.

