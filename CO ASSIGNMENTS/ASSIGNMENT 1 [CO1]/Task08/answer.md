# Task 8 — Read and print data safely

`safe_io.cpp` checks whether `std::cin >> value` succeeds. If letters are entered where an integer is expected, it prints an error, calls `cin.clear()` to reset the stream state, and discards the rest of the bad line with `cin.ignore(...)` before asking again. It then rejects hours outside 0–23 and negative recharge amounts. The multiplication by 100 is done after casting to `long long`, so rupees become paise without integer overflow at the `int` width.

The valid transcript enters hour 8 and recharge Rs 250; the program prints `Accepted recharge = Rs 250.00 (25000 paise)`. The invalid transcript enters `abc`, which is rejected and retried, then a negative recharge, which is rejected with exit code 1. Both real runs are saved in `valid_input_output.txt` and `invalid_input_output.txt`.

Integer quotient/remainder formatting prints money exactly: `paise / 100` is rupees and `paise % 100` is the two-digit paise part. `<iomanip>` manipulators such as `setw(2)` and `setfill('0')` can align the fractional part; `fixed` and `setprecision(2)` are appropriate only when formatting a floating-point display value, not for storing or calculating money.

An alternative is `getline` followed by `stoi` inside `try/catch`. Both approaches scan a line/token in O(L) time for input length L; a string buffer needs O(L) extra space. `getline` gives clear whole-line validation (including trailing characters), while formatted extraction is concise for whitespace-separated input. Both must define recovery behavior for invalid text and end-of-file.
