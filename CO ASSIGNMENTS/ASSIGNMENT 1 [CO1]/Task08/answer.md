# Task 8 — Read input safely and print a journey report

`safe_io.cpp` reads a zone number, departure hour, and card balance. If text is entered where an integer is expected, it reports the error, calls `cin.clear()` to reset the stream state, and discards the bad line with `cin.ignore(...)` before retrying. It also re-prompts for a zone outside 1–3, an hour outside 0–23, or a negative balance.

After validating the zone, the program selects the base fare, applies the 20% peak surcharge only from 08:00–09:59 or 17:00–19:59, and checks the Rs 50 minimum-balance rule. The report prints the zone, departure time, fare, approval status, and remaining balance. The valid sample `1, 8, 100` produces an Inner-zone peak fare of Rs 12.00, approves the trip, and reports Rs 88.00 left.

Money is stored and calculated as integer paise. `printMoney` uses quotient and remainder, `setw(2)`, and `setfill('0')` so the output always has two paise digits. The rupee input is converted to `long long` before multiplying by 100. Students and senior citizens are not given a concession rate in the assignment, so this task does not invent one.

The valid and invalid/recovery input files and their run records are saved beside the source. In the recovery run, `abc` is rejected and the program continues with the next valid inputs. A separate `getline` plus `stoi` approach takes O(L) time for a line of length L and O(L) buffer space; it can reject trailing characters more explicitly, while formatted extraction is concise for whitespace-separated values. Both methods need a defined recovery path for invalid input and end-of-file.
