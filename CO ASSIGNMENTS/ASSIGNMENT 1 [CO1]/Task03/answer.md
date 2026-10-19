# Task 3 — Choose data types and check compatibility

| System value | Chosen type | Why |
|---|---|---|
| Fare and card balance | `long long` integer paise | Exact money arithmetic; enough range for the whole network total |
| Hour and percentage | `int` | Small whole values; signed input lets the program reject negatives |
| Zone | `enum class Zone` | Gives named zones and prevents accidental integer mixing; parse external numeric input as `int`, validate it, then convert |
| Journey count and accumulated total | `long long` | Represents millions of journeys and totals without 32-bit overflow |
| Peak/approval flag | `bool` | Two states; arithmetic promotes it to 0 or 1 |
| A single character | `char` | Suitable for a character or code unit, not for a fare or zone model |
| Non-negative count alternative | `unsigned int` | Its maximum is larger than `int`'s, but input should first be validated as signed; unsigned subtraction can wrap, so it is a poor choice for balances |
| Floating-point comparison | `float` / `double` | Demonstration or approximate measurements only; neither should store fares or balances |
| Local value with an obvious initializer | `auto` | Deduces the initializer's type at compile time; for example, `auto journeyCount = 3000000LL` is still a `long long`, not a new runtime numeric type |

On the compiler used here, `types.cpp` measures the size of every requested type, including `unsigned int` and the `auto` variable. It also prints the ranges for `char`, `int`, `unsigned int`, and `long long`, plus the largest finite `float` and `double`. These sizes are implementation-dependent; read the measured values in `RUN_OUTPUT.txt` rather than assuming every compiler uses the same layout.

Money is stored as integer paise because decimal hundredths generally have no exact binary floating-point representation. Repeated addition with `double` can accumulate rounding error; Task10 measures this effect. At the maximum peak fare of Rs 36, three million journeys total Rs 108,000,000, or **10,800,000,000 paise**. That exceeds the tested `INT_MAX` of 2,147,483,647, so a 32-bit `int` is not enough for the network paise total. The tested `long long` range is sufficient.

In mixed expressions, C++ applies the usual arithmetic conversions. For example, `int / int` performs integer division even if the result is later assigned to `double`; a signed/unsigned expression can convert the signed value to unsigned. Prefer validating signed user input before converting it to a non-negative domain type.

**Complexity comparison:** one addition is O(1) with integer paise or `double`; each 64-bit total takes 8 bytes on this compiler. Integer paise is exact within its range, while `double` has a wider range but approximate decimal values. See `types.cpp` and `Task10/scale.cpp` for measured sizes, ranges, and precision results.
