# Task 5 — Operators and precedence

## Prediction and actual output

The reference prediction was recorded in `MY_PREDICTION.txt` before running the program. The actual output in `RUN_OUTPUT.txt` is:

```text
11 16
9 6 4
6 4
3 1 -3 -1
18
```

It matches the prediction.

## Explanation

- `x + y * 2` multiplies before adding: 5 + (3×2) = 11. Parentheses force `(x+y)*2` = 16.
- `x++` yields the old value 5 and then increments `x`; `++y` increments `y` first and yields 4. Thus `z=9`, then `x=6`, `y=4`. The operands modify different objects, so this expression is well-defined; avoid modifying the same object multiple times in an unsequenced expression.
- The conditional operator `x > y ? x : y` chooses 6. `%` and `*` have equal precedence and associate left to right, so `(6 % 4) * 2 = 4`.
- Integer division truncates toward zero: `10/3=3` and `-10/3=-3`. The remainder has the sign of the dividend: `10%3=1`, `-10%3=-1`.
- The loop visits `i=0,3,6,9`; the accumulated sum is 18.

The peak expression is `(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)`. Relational operators bind before `&&`, and `&&` binds before `||`; parentheses make the two intervals clear. The surcharge can be written `fare += fare * percent / 100`, and concession as `fare = fare * (100 - discountPercent) / 100`, with a documented order of application and integer-paise arithmetic. `i = i++` is not a safe way to increment: in C++11 it has undefined behavior due to unsequenced modification. Use `++i` or `i += 1`.

A 24-entry `bool peak[24]` table also provides O(1) lookup, using about 24 bytes when `sizeof(bool)==1`; the expression uses O(1) space too. The expression avoids table initialization, while the table can make policy data explicit.
