# Task 4 — Control implicit conversions

## Prediction and actual output

The reference prediction was recorded before the run in `MY_PREDICTION.txt`.

```text
3 3.5
C 68
1 true
s >= u
```

The actual run in `RUN_OUTPUT.txt` matches the prediction.

## Conversion points

1. `a / b` is evaluated as `int / int`, so 7 / 2 truncates to 3. Assigning 3 to `double c` converts the integer result to 3.0. In `static_cast<double>(a) / b`, `a` is converted before division, so the result is 3.5.
2. In `'A' + 2`, the character is promoted to `int`; the result is converted back to `char` when stored in `ch`, giving `C`. In `ch + 1`, `ch` is promoted to `int`, so the stream prints 68, the code point value of `D` on this system.
3. Assigning integer 5 to `bool` converts any nonzero value to `true`. The default stream representation is `1`; `std::boolalpha` prints `true`.
4. In `s < u`, the negative signed `int` is converted to `unsigned int` by the usual arithmetic conversions. On this 32-bit `unsigned int` implementation, -1 becomes 4294967295, so the comparison is false and the program prints `s >= u`. `-Wsign-compare` correctly warns about mixing signed and unsigned operands.

Use `static_cast` when a conversion is intentional and visible. Brace initialization such as `int count{3.7};` is rejected because it would narrow a `double` to an `int`; an explicit cast documents a deliberate conversion. Prefer types that avoid unsafe mixed-sign comparisons.

If all conversions were explicit, the program would make data flow clearer and brace initialization would reject narrowing at compile time. Casts do not change the O(1) runtime work or storage for these scalar values; they help expose mistakes during compilation rather than waiting for surprising runtime results.
