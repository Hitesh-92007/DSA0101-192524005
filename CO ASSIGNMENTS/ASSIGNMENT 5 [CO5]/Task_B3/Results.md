# Task B3 — Measurements and Engineering Trade-offs

## Method

Three implementations compute the same deterministic records: virtual `Bill` objects referenced through a base-pointer array, a tagged record with `switch`, and `std::variant` with `std::visit`. For the listed timings, data construction is outside the timed loop, each reported value is the average over the pass count in the CSV output, and a volatile sink retains each computed result. The 10-bill sample is also checked against a hand-computed total of 89,940. The run used `-O2`, C++17 and the toolchain version in the root README. Results are one measured run from this machine; they are not universal guarantees.

The switch implementation performs a switch during each bill total. The virtual implementation traverses a base pointer list. The variant implementation dispatches through `std::visit`. Dataset allocation and object construction are excluded, so these values compare total-loop cost rather than full end-to-end bill ingestion.

## Billing totals and timing

See `benchmark.csv` and `Output_Screenshots/benchmark.txt`. All models produced equal totals at 10, 1,000, 1,000,000 and 10,000,000 bills. Average per-pass measurements (milliseconds) are copied from the executed run.

| Bills | Virtual | Switch | Variant | Passes |
|---:|---:|---:|---:|---:|
| 10 | 0.000017 | 0.000008 | 0.000006 | 10,000 |
| 1,000 | 0.001583 | 0.000737 | 0.000662 | 1,000 |
| 1,000,000 | 1.692960 | 1.989720 | 1.656260 | 5 |
| 10,000,000 | 17.544150 | 22.761900 | 18.875000 | 2 |

## Error-handling timings

For each 1,000,000 claims, the same deterministic input is validated with an enum return code and with typed exceptions. The invalid claims are negative amounts; both implementations report the same number of failures.

| Invalid fraction | Return-code time (ms) | Exception time (ms) | Invalid cases from both |
|---:|---:|---:|---:|
| 0% | 0.828400 | 1.115600 | 0 |
| 1% | 0.823400 | 29.932200 | 10,000 |
| 50% | 0.734100 | 1,392.754400 | 500,000 |

## Interpretation

Virtual, switch and variant all have `O(n)` total time and `O(n)` data storage. Relative time depends on object layout, compiler and workload; this run shows no universally winning dispatch choice at all sizes. The virtual design is easiest to extend when types are defined in code by separate teams; the switch is direct and can have good locality but centralizes every type test; `std::variant` makes the supported type set explicit and avoids a base pointer per object, but adding an alternative changes the variant type and visitors. A variant element occupies the storage of its largest alternative plus a discriminator and alignment padding.

Return-code validation has nearly constant time as the invalid fraction rises. Exceptions are comparable on valid data in this run but become much slower when thrown frequently, especially at 50% invalid. Use exceptions for exceptional failures that cross layers; use a return value such as `std::optional`/a result type for expected high-volume rejection. A result object costs `O(1)` extra space per result; when streaming, that result need not be retained in a vector.
