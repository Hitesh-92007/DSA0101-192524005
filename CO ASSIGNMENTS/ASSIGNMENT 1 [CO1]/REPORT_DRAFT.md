# Assignment 1 [CO1] — Report Draft

Student: [Name]
Register number: [Register number]
GitHub link: [Assignment folder in the selected subject repository](https://github.com/Hitesh-92007/DSA0101-OOPS-WITH-C-/tree/main/CO%20ASSIGNMENTS/ASSIGNMENT%201%20%5BCO1%5D)
Compiler/version: g++ (tdm64-1) 4.9.2, C++11

Add your name and register number. Record and date your own predictions for Tasks 4 and 5 before running those programs. The other available local measurements and run results are recorded below and in the task folders. Add the screenshots required by the PDF and any personal evidence that only you can provide.

## Assumptions

The assignment does not specify the student/senior concession percentage. This draft assumes 50% and makes that assumption replaceable; confirm the rule with the faculty. Peak windows are start-inclusive/end-exclusive: 08:00–09:59 and 17:00–19:59. Money is integer paise. Task 6's supplied program uses only base fare and peak surcharge. In the general example, surcharge is applied before concession. Minimum post-trip balance is 5000 paise.

## Task 1 — OOP analysis

SmartCard stores an ID and private balance; canTravel and deduct enforce the minimum-balance rule. Passenger stores a passenger category and determines concession eligibility. Journey stores zone and time. Zone represents Inner, Middle, Outer, and later Airport. FareTable stores policy values and calculates fare. In procedural code, state is separate and free functions receive it as parameters; OOP groups data and operations. Encapsulation hides balance, abstraction exposes fare/card operations, inheritance could represent passenger types, and polymorphism could select concession policies. Inheritance is not necessary for this small system; a category enum or strategy is simpler. Rules: reject invalid inputs; balance may never go negative; successful journeys leave at least Rs 50; use exact paise; apply peak surcharge only inside the defined ranges. Work and memory per journey/card are O(1). Centralized fare policy requires fewer edits when a rule changes.

### UML relationship sketch

SmartCard (private balance; canTravel, deduct) 1 ---- 0..* Journey
Journey (zone, time, day) ---- asks fare from ---- FareTable
Passenger (category) ---- concession rule used by ---- FareTable
Zone (Inner, Middle, Outer, Airport) ---- key used by ---- FareTable

## Task 2 — Tokens and constants

Examples: keyword switch; identifier peakHour; constant 20; operator >=; punctuator semicolon. A macro is textual substitution with no type/scope checks. const creates a non-modifiable object; constexpr permits compile-time evaluation. Prefer constexpr for compile-time fare values. enum class scopes zone names and disallows accidental integer conversion. Use descriptive lowerCamelCase for values/functions and PascalCase for types; never use reserved words. Replacing constexpr/enum class with macros loses type and scope checking. Compile-time constants normally require no runtime storage; runtime cost is effectively unchanged.

## Task 3 — Types and compatibility

Use bool for flags; enum class Zone for zones; int for hours and percentages; long long for paise totals and journey counts; std::string for IDs. Store money as integer paise because binary floating point cannot represent many decimal fractions exactly. A typical signed 32-bit int can hold whole-rupee totals at 3 million journeys, but not 10,800,000,000 paise (3 million times the maximum peak fare of 3600 paise). Use long long. On the tested 64-bit Windows setup, Task03 printed: `bool 1 char 1 int 4 long long 8 float 4 double 8 bytes`; `INT_MIN = -2147483648`, `INT_MAX = 2147483647`; maximum network total = 10,800,000,000 paise; ten additions of 0.1 = 0.99999999999999989.

## Task 4 — Conversions

The first division is integer division (7 / 2 becomes 3) before conversion to double. The explicit cast makes the second division floating point. Character arithmetic promotes char to int; assigning the result back narrows it to char, and streaming ch + 1 prints an integer. Assigning 5 to bool yields true; default output is 1 and boolalpha prints true. In the signed/unsigned comparison, -1 converts to unsigned, so on a usual 32-bit unsigned int system the comparison is false and the program prints s >= u. Brace initialization rejects narrowing; explicit casts document intended conversion.

My prediction before running: [write and date it before execution in Task04/MY_PREDICTION.txt]  
Actual output and explanation: [paste your run and explain each line]

## Task 5 — Operators

Peak check: (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20). Surcharge: fare += fare * percent / 100. Concession: fare = fare * (100 - discountPercent) / 100. Parentheses clarify intent. Post-increment yields the old value; pre-increment yields the updated value. Do not modify a variable repeatedly in one expression. Integer division truncates toward zero; remainder has the sign of the left operand. The peak-range expression is O(1) time and O(1) space; a 24-entry boolean table is also O(1) lookup but uses a small fixed table.

My prediction before running: [write and date it before execution in Task05/MY_PREDICTION.txt]  
Actual output and operator-by-operator explanation: [complete after execution]

## Task 6 — Control flow

Pseudocode: validate zone/hour/category; select base fare; add 20% in peak hours using integer paise arithmetic; apply the documented concession; deny without changing balance if balance minus fare is below 5000; otherwise deduct and print fare and balance. Use switch for a small fixed enum, if for ranges, range-based for for known trip lists, and while for retrying invalid input. Include a default branch for invalid enum values. Task06/fare_complete.cpp fills the supplied TODOs. Add explicit tests at exactly 10:00 and 20:00; these are off-peak.

Local run: Inner off-peak cost Rs 10.00 and left Rs 90; Outer peak cost Rs 36.00 and left Rs 54; the next Middle trip was denied because it would leave less than the required Rs 50. The run output is in `Task06/RUN_OUTPUT.txt`.

Array lookup and switch are both O(1); a table uses a small fixed amount of memory. Validate enum values before indexing. Explain how adding Airport changes the switch/table.

## Task 7 — Procedural vs OOP

Task07 contains both implementations. Both have O(1) affordability/deduction and O(1) per-card space. Private balance plus methods centralize the invariant; public procedural state relies on every caller following the rule. Centralized design is easier to revise when minimum balance changes. With a starting balance of 10000 paise and a 3600-paise fare, both programs print `Allowed; remaining paise = 6400`.

## Task 8 — Input/output

safe_io.cpp checks stream failure, clears the failure state, and discards the bad line before retrying. It validates hour and negative recharge. Integer quotient/remainder prints paise exactly; iomanip can format decimal values. getline plus stoi is O(L) for a line of length L and uses O(L) buffer space; it can reject trailing garbage more deliberately, while formatted extraction is concise for tokens. A valid `8`, `250` input was accepted as 25000 paise. Nonnumeric hour input was rejected and retried; a negative recharge was rejected. Transcripts are in the Task08 folder.

## Task 9 — Implementation/testing

Task09/fare_tests.cpp includes cases for zones, boundaries, invalid zone/hour, exact Rs 50 remaining, negative recharge, and a hand-checkable five-trip total. Add your actual results:

| Case | Expected | Actual | Pass |
|---|---|---|---|
| Inner off-peak | 1000 paise | 1000 paise | Pass |
| Middle off-peak | 2000 paise | 2000 paise | Pass |
| Outer off-peak | 3000 paise | 3000 paise | Pass |
| 08:00 / 10:00 | 1200 / 1000 paise | 1200 / 1000 paise | Pass |
| 17:00 / 20:00 | 3600 / 3000 paise | 3600 / 3000 paise | Pass |
| Exact Rs 50 left | allowed | allowed | Pass |
| Invalid zone/hour and negative recharge | rejected | rejected | Pass |
| Five-trip total | 10800 paise | 10800 paise | Pass |

The test program reports 9 named cases passed and `Failures = 0`. Full output is in `Task09/RUN_OUTPUT.txt`.

Testing t rows is O(t) time and O(t) table storage. Add one row for each new boundary case.

## Task 10 — Scale analysis

At Rs 36 maximum peak fare, totals are Rs 720,000, Rs 18,000,000 and Rs 108,000,000 for 20,000, 500,000 and 3,000,000 trips. In paise the last is 10,800,000,000, beyond a typical 32-bit signed int. A long long paise total is appropriate. Task10 compares repeated double accumulation with exact integer paise using Rs 12.34: for 20,000 trips the difference was about -0.0000000602 rupees; for 500,000, about -0.0000429647; for 3,000,000, about +0.0006307215. `INT_MAX` was 2147483647 and 10,800,000,000 paise did not fit. The output is in `Task10/RUN_OUTPUT.txt`. Compilation produced one expected signed/unsigned comparison warning in Task04; other files compiled without diagnostics.

## Task 11 — Recommendation

Use integer paise in long long, enum class zones, a centralized fare policy, validated input and a SmartCard with private balance. This keeps per-journey work O(1) and all journeys O(n). Add a fourth zone in one policy table. Limitations: concession stacking and pass policy need exact business rules; persistence, security and concurrency require additional design. Support this recommendation with your own Task09/10 measurements. A configuration file adds O(z) startup read time/storage for z fares but avoids recompilation for each fare revision; validate file contents and define safe failure behaviour.

## Task 12 — Day-4 challenge

Add Airport at Rs 60, a 10% weekend discount that replaces peak surcharge, and a MonthlyPass covering two selected zones. Retain SmartCard, but modify fare selection to a validated table and add a pass policy. Examples: Airport weekday peak is Rs 72; Airport weekend during peak time is Rs 54; Airport covered by pass is free. Task12 demonstrates this. The local run printed 7200, 5400 and 0 paise respectively; the pass trip was accepted and balance remained 20000 paise. The code preserves the Rs 50 minimum even for a zero-fare pass journey; confirm this interpretation with the faculty. Output is in `Task12/RUN_OUTPUT.txt`.

## Conclusion

The recommended design centralizes fare rules, uses typed zones and integer paise, validates input, and protects card balance. Before submission, add your name/register number, write and date personal predictions for Tasks 4 and 5 before running them, add required screenshots and any required UML diagram, and confirm the instructor accepts the selected subject repository.
