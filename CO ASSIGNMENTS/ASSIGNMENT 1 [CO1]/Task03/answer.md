# Task 3 — Choose data types and check compatibility

| Value | Suggested type | Reason |
|---|---|---|
| Fare and card balance | `long long` paise | Exact cents/paise arithmetic and room for network totals |
| Hour and percentage | `int` | Small bounded whole numbers |
| Zone | `enum class Zone` | Named, restricted values rather than an unvalidated integer |
| Journey count | `long long` | Safely represents millions and larger totals |
| Success/failure | `bool` | Two-state result |
| Single character | `char` | Character data only, not monetary data |
| Approximation experiment | `double` | Useful for measuring floating-point error, not for storing fares |
| Generic local value | `auto` | Deduces the initializer's type at compile time; it is not a separate runtime type |

On the Windows x64 compiler used for the run, `sizeof` reported: `bool=1`, `char=1`, `int=4`, `long long=8`, `float=4`, and `double=8` bytes. `unsigned int` is also 4 bytes on this compiler. The exact sizes are implementation-dependent except for the minimum guarantees in the C++ standard.

Money should be stored as integer paise because many decimal fractions have no exact binary floating-point representation. Repeatedly adding a `double` can accumulate rounding error; integer paise additions are exact while the value stays in range. In the measurement in Task10, adding Rs 12.34 three million times as `double` differed from the exact paise total by about Rs 0.0006307.

The run reported `INT_MIN=-2147483648` and `INT_MAX=2147483647`. At the maximum peak fare of Rs 36, three million journeys total Rs 108,000,000, or **10,800,000,000 paise**, which exceeds `INT_MAX`; a 32-bit `int` is not enough for that paise total. A signed 64-bit `long long` is suitable. In mixed expressions, the usual arithmetic conversions choose a common type; for example, an `int` divided by an `int` is integer division even if assigned afterward to `double`. A `bool` converts to 0 or 1 in arithmetic, and a `char` is promoted to `int` in arithmetic expressions.

**Complexity comparison:** adding one fare is O(1) with integer paise or `double`, and each stored total uses 8 bytes here. `double` is approximate; integer paise is exact within its range. The `Task03/types.cpp` and `Task10/scale.cpp` runs provide the measured sizes, ranges, and precision comparison.
