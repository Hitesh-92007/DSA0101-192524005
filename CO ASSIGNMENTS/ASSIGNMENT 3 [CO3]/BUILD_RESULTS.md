# Build and verification results

## Toolchain

- `g++` driver from LLVM-MinGW; reported compiler: Clang 23.1.1
- Target: `x86_64-w64-windows-gnu`
- Standard: C++17
- Normal flags: `-std=c++17 -Wall -Wextra -Wconversion -pedantic -O2`
- Sanitizer flags: `-std=c++17 -Wall -Wextra -pedantic -O1 -g -fsanitize=address -fno-omit-frame-pointer`

All seven normal programs compiled and ran. Q1 intentionally emits a member-initializer reorder warning. The regular Q2 and Q6 builds are warning-free after the self-assignment checks were written without triggering a compiler self-assignment warning.

## Expected compile diagnostics

- `Task01/q1_explicit_probe.cpp` compiles without `EXPLICIT_CONSTRUCTOR`; with `-DEXPLICIT_CONSTRUCTOR`, `PageCount pages = 300` fails because copy-initialization cannot use an explicit constructor.
- `Task01/q1_no_default_array.cpp` is expected to fail because `Book books[2]` needs a default constructor that the class does not define.
- `Task01/q1_book_constructors.cpp` uses `ReorderProbe` to produce the expected `-Wreorder-ctor` diagnostic. Its actual state remains `1 2` because declaration order controls initialization.

The full diagnostics are in `Task01/BUILD_RESULTS.txt`, `Task01/EXPLICIT_COMPILE_DIAGNOSTIC.txt`, and `Task01/NO_DEFAULT_ARRAY_DIAGNOSTIC.txt`.

## Sanitizers

- Q2 correct deep-copy build: AddressSanitizer run exits 0.
- Q2 `-DSHALLOW_VERSION`: AddressSanitizer exits with an intentional double-free report when the two aliased objects are destroyed.
- Q3 correct event build: AddressSanitizer run exits 0.
- Q3 `-DMISSING_DELETE`: program exits normally, but the Windows runtime says `detect_leaks is not supported on this platform`. LeakSanitizer could not confirm the leak in this environment; the missing `delete` is deliberate and shown in the source and transcript.
- Q6 Text build: AddressSanitizer run exits 0.

Detailed logs are in the Q2, Q3, and Q6 `SANITIZER_RESULTS.txt` files. The machine has no WSL distribution, so a Linux LeakSanitizer rerun was unavailable.

## Timing summary

The figures below are five separate process runs on the available Windows toolchain. They are measurements for this machine, not portable performance guarantees.

| Measurement | Median | Observed range |
|---|---:|---:|
| Q4, 1,000,000 integer-paise additions | 1,001 μs | 693–1,337 μs |
| Q4, 1,000,000 double additions | 3,219 μs | 2,584–4,829 μs |
| Q5, 10,000,000 prefix increments, `-O0` | 56,888 μs | 56,424–62,500 μs |
| Q5, 10,000,000 postfix increments, `-O0` | 58,627 μs | 56,304–76,876 μs |
| Q5, 10,000,000 prefix increments, `-O2` | 54,184 μs | 52,921–60,570 μs |
| Q5, 10,000,000 postfix increments, `-O2` | 52,531 μs | 48,927–70,221 μs |
| Q6, 100,000 `Text` concatenations, `-O2` | 5,086 μs | 4,836–6,259 μs |
| Q6, 100,000 `std::string` concatenations, `-O2` | 3,607 μs | 3,567–5,319 μs |

Raw measurements are preserved in `BENCHMARK_RUNS.txt`. The Q5 increment timings overlap and vary; in this benchmark the postfix return copy is small and the optimizer/runtime noise outweigh a consistent difference. For a class whose postfix result is unused, prefix avoids constructing and returning the old value by design. Q6's median favors `std::string`, with run-to-run variation visible in the raw log.
