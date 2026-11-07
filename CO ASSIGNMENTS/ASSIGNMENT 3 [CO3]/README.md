# Assignment 3 [CO3] — Constructors, destructors, and operator overloading

**Student:** Hitesh  
**Register number:** 192524005  
**Compiler:** LLVM-MinGW `g++` driver, Clang 23.1.1, target `x86_64-w64-windows-gnu`  
**Language standard:** C++17

This folder contains the seven C++ tasks, written answers, run records, sanitizer records where applicable, benchmark measurements, and the complete report in [`Report/Report.md`](Report/Report.md). Each task has its own folder.

## Repository note

The assignment PDF asks for a separate repository named `DSA01-<RegisterNumber>-Assignment3`. This work is being prepared in the existing `DSA0101-192524005` repository because that is the repository selected for the task. Please confirm with the course faculty that this repository name is accepted for submission.

## Build and run

Run commands from the relevant task folder in a terminal where `g++` is available. The normal build uses `-std=c++17 -Wall -Wextra -Wconversion -pedantic -O2`.

| Task | Source | Purpose |
|---|---|---|
| Q1 | `Task01/q1_book_constructors.cpp` | Default, title-only, and full Book construction; invalid count and reorder warning |
| Q1 probe | `Task01/q1_explicit_probe.cpp` | Compile once normally and once with `-DEXPLICIT_CONSTRUCTOR`; the explicit build should fail |
| Q1 probe | `Task01/q1_no_default_array.cpp` | Expected compile failure after removing the default constructor |
| Q2 | `Task02/q2_prescription_deep_copy.cpp` | Rule-of-Three implementation; compile with `-DSHALLOW_VERSION` for the intentional shallow-copy failure |
| Q3 | `Task03/q3_auditorium.cpp` | Dynamic seat maps; compile with `-DMISSING_DELETE` for the intentional leak case |
| Q4 | `Task04/q4_money_billing.cpp` | Paise-based arithmetic, comparison, and stream operators |
| Q5 | `Task05/q5_time_playlist.cpp` | Prefix/postfix increment, comparison, checked playlist access, and benchmark |
| Q6 | `Task06/q6_text.cpp` | Raw-buffer Text class, deep copies, operators, and `std::string` comparison |
| Q7 | `Task07/q7_complex_impedance.cpp` | Complex arithmetic and series/parallel RLC impedance |

Example commands (PowerShell):

```powershell
g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q1_book_constructors.cpp -o q1.exe
.\q1.exe

g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic -O2 q2_prescription_deep_copy.cpp -o q2.exe
.\q2.exe
g++ -std=c++17 -Wall -Wextra -pedantic -O1 -g -fsanitize=address -fno-omit-frame-pointer q2_prescription_deep_copy.cpp -o q2_asan.exe
```

For Q5, compile and run once with `-O0` and once with `-O2`. For Q2 and Q3, the shallow-copy and missing-delete variants are deliberate fault demonstrations; the correct versions are separate default builds. The actual command set and expected diagnostic outcomes are recorded in [`BUILD_RESULTS.md`](BUILD_RESULTS.md). Output transcripts are stored in each task folder.

## Task map

| Task | Main result |
|---|---|
| Q1 | Constructor delegation initializes three library record states; negative copy counts are rejected; the compiler demonstrates `explicit`, missing default construction, and reorder warnings. |
| Q2 | Deep copies remain independent; self-assignment is safe; AddressSanitizer catches the shallow-copy double free. |
| Q3 | Stack, block, and heap events release seat maps at the correct lifetime boundary; the seat-map payload estimates are 280 B, 20,800 B, and 2,008,000 B. |
| Q4 | Paise arithmetic prints Rs 200.00, carries Rs 0.75 + Rs 0.50 to Rs 1.25, and represents negative subtraction exactly. |
| Q5 | The expected prediction is `5 7 7`; checked indexing rejects -1 and `size`; rollovers work. |
| Q6 | Raw-buffer text copies are independent; C++17 elides the returned copies in `a+b+c`; the five-run median for `std::string` was faster in this local benchmark. |
| Q7 | Series impedance is 10.0000−j0.4151 Ω and parallel impedance is 9.9998+j0.0415 Ω; zero division is rejected. |

## Evidence and integrity

Each task has a `PREDICTION.md` prepared by the assistant before the final verification rerun. Exploratory executions happened earlier, so these are not personal, independently authored predictions made before the first run. All seven programs were rerun; both Q5 optimization levels and the other six task runs exited successfully. Updated stdout transcripts are included. A genuine Notepad capture of Q1 output appeared in the conversation, but no screenshot image files are included in this package; add the source/output screenshots requested by the assignment if the instructor requires them. The Windows AddressSanitizer runtime supports address checks but reports that LeakSanitizer is unavailable; therefore the intentional missing-delete run does not produce a leak report. The code and transcript make that omitted `delete` visible.

This assignment was prepared with AI coding and writing assistance. Hitesh should review every explanation and be able to explain the implementation during the verification discussion. No personal lab execution or independent authorship is claimed here.
