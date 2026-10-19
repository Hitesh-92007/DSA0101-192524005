# Task 2 — Tokens, keywords, identifiers, and constants

## One named set of fare rules

```cpp
enum class Zone { Inner, Middle, Outer };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
```

These values match the scenario: Rs 10, Rs 20, and Rs 30 are stored as 1000, 2000, and 3000 paise; the peak surcharge is 20%; and a card must retain 5000 paise. A fare revision is made in this one policy definition.

## Five token kinds

Using the snippet above: `constexpr` is a **keyword**; `BASE_PAISE` is an **identifier**; `1000` is a **constant literal**; `=` and `*` are **operators**; and `{`, `,`, and `;` are **punctuators**. Tokens combine to form declarations and expressions.

## Constant and zone choices

`#define` performs textual substitution before compilation and has no type or ordinary scope checking. `const` creates a read-only object with a type. `constexpr` declares a value usable in constant expressions, so the compiler can validate and evaluate fare values at compile time. A compile-time constant usually requires no separate runtime storage and does not make a fare lookup slower. `enum` exposes its enumerator names in the surrounding scope and can convert to an integer; `enum class` scopes the names and disallows implicit integer conversion, reducing accidental zone mix-ups.

Use PascalCase for types (`SmartCard`, `FareTable`), lowerCamelCase for variables/functions (`balancePaise`, `calculateFare`), and uppercase snake case for named constants (`MIN_BALANCE_PAISE`). Identifiers must start with a letter or underscore, contain only permitted letters/digits/underscores, and must not be a C++ keyword; avoid reserved underscore patterns.

If the values were macros, the program would lose type and scope checking, while memory and runtime lookup cost would remain effectively unchanged. For this fixed table, a `constexpr` array plus `enum class` gives O(1) fare lookup and a small fixed amount of storage.
