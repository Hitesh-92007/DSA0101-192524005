# Task 9 — Implement and test the fare logic

The source `fare_tests.cpp` runs a table of expected-versus-actual fare cases and checks the card and input rules. The actual run completed with **Failures = 0**.

| Case | Expected | Actual | Result |
|---|---:|---:|---|
| Inner, Middle, Outer off-peak | 1000, 2000, 3000 paise | 1000, 2000, 3000 paise | Pass |
| Inner at 08:00 | 1200 paise | 1200 paise | Pass |
| Inner at 10:00 | 1000 paise | 1000 paise | Pass |
| Middle and Outer at 17:00 | 2400, 3600 paise | 2400, 3600 paise | Pass |
| Outer at 20:00 | 3000 paise | 3000 paise | Pass |
| Invalid hour / unknown zone | Rejected | Rejected | Pass |
| Balance 6200 paise paying 1200 | Allowed; leaves 5000 | Allowed | Pass |
| Negative recharge | Rejected | Rejected | Pass |
| Nonnumeric hour `abc` | Rejected | Rejected by the input parser | Pass |
| Five fares: 1000+2000+3600+3000+1200 | 10800 paise | 10800 paise (Rs 108.00) | Pass |

The detailed console record is `RUN_OUTPUT.txt`; planned cases are in `test_plan.csv`. Using a table makes the suite easy to extend: for `t` test cases, execution is O(t) time and the table uses O(t) space. A boundary case can be added as one row instead of a new hand-written branch.

The test program uses integer paise and checks both sides of the peak boundaries: 08:00/10:00 and 17:00/20:00. These intervals are start-inclusive and end-exclusive. The current table directly tests the input parser with `abc`; Task08 also has a run transcript showing the prompt, rejection, and retry.
