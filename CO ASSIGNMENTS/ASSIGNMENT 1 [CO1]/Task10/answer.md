# Task 10 — Compare expectations with execution

`scale.cpp` adds Rs 12.34 repeatedly using both integer paise and `double` rupees. The observed results were:

| Journeys | Exact paise total | `double` total (Rs) | `double - exact` (Rs) |
|---:|---:|---:|---:|
| 20,000 | 24,680,000 | 246799.99999993978 | -0.0000000602158 |
| 500,000 | 617,000,000 | 6169999.9999570353 | -0.0000429647043 |
| 3,000,000 | 3,702,000,000 | 37020000.000630721 | +0.00063072145 |

The double result does not stay exact; binary floating point represents most decimal hundredths approximately, so repeated additions accumulate small error. The measured error is small in these examples but increases in magnitude as more values are accumulated. Integer paise remains exact as long as the integer type does not overflow.

For the maximum fare scenario, 3,000,000 × Rs 36 = Rs 108,000,000 = 10,800,000,000 paise. This exceeds the 32-bit `INT_MAX` of 2,147,483,647. Signed `int` overflow in C++ is undefined behavior, so it must not be relied on. Use 64-bit `long long` for the total. `long long` and `double` are both 8 bytes on the tested compiler; an `int` is 4 bytes. One addition is O(1) for each type, and summing n journeys is O(n).

With `int`, the network paise sum cannot be represented safely. With `long long`, this example fits exactly. With `double`, it has a much wider range than `int` but can lose fractional precision. The run record in `RUN_OUTPUT.txt` includes each measured value and the range check. Compilation with `-Wall -Wextra -Wconversion -pedantic` produced no diagnostics for `scale.cpp`; Task04's intentional signed/unsigned comparison produced a warning documented there.
