# Q6 - Friend functions and inline functions

## Why the comparison is a friend

`minutesBetween(a, b)` and `earlier(a, b)` treat the two `Time` values symmetrically. They are non-member functions, so their calls read naturally with both times as arguments. Friendship lets them read private `h` and `m` without making those fields public. A member version such as `a.minutesTo(b)` could also access private data and may be a better fit if the operation is considered behaviour of one `Time` object. Public getters keep the class encapsulated but add accessors. Friendship should be limited to functions that need direct access.

Making `h` and `m` public would let every caller create invalid values such as hour 30 or minute 90, so it weakens the class invariant. A robust larger class could validate inputs in its constructor.

## Correctness

The functions convert each time to minutes after midnight and compare/subtract those values. The supplied test prints `205 false`. A test spanning noon (`11:55` and `12:10`) prints a 15-minute difference and says the first time is earlier. Comparing `14:05` with another `14:05` prints difference 0 and `false` for earlier.

## Inline measurement

In C++, `inline` permits equivalent definitions in multiple translation units and suggests that a call may be expanded at the call site; it does not require the compiler to do so. The compiler may inline a function without the keyword. Each build below ran 10,000,000 calls with a fixed set of 1,024 generated time pairs; the checksum was identical. The four measurements are from this machine, using the `g++` driver for LLVM-MinGW/Clang 23.1.1, C++17. Timings vary with system load.

| Build | Median of 5 runs (microseconds) |
|---|---:|
| No `inline`, `-O0` | 164,112 |
| `inline`, `-O0` | 125,588 |
| No `inline`, `-O2` | 17,484 |
| `inline`, `-O2` | 11,420 |

In these five-run medians, the inline-labelled build is faster in both settings, but the `-O2` improvement is much larger and system load can change close comparisons. The keyword is still not a guarantee that a call is expanded; the compiler decides. The function work is O(1) per call and uses O(1) extra space. The five runs per variant are saved in the `q6_*_runs.txt` files; the exact compile commands are in the top-level README.
