# Task B4 — Final Engineering Decision

## Extension

`production_extension.cpp` adds `TelemedicineBill` and `CorporateTieUp` as new classes derived from the existing `Bill` and `Payer` interfaces. `hospital_billing.hpp`, the existing bill types, payer types and `BillingSystem` are unchanged. The new bill is added to the same `BillingSystem` list and settled by the new payer.

For a consultation of 800 and a remote test of 250, the bill is 1,050. A 90% corporate policy with a 50,000 limit covers 945; patient due is 105. Existing B1 outputs remain unchanged. The extension also throws and catches a `ClaimError` while a stack guard is alive; the guard prints its release message during unwinding.

## AddressSanitizer

The B4 executable was compiled with `-fsanitize=address` and run with a caught exception. It completed without an AddressSanitizer memory-error report. This Windows runtime says LeakSanitizer is unsupported, so `detect_leaks=0` was used; leak detection itself could not be claimed.

## Production decision

For 15 hospitals, two bill types and three payers added annually, and 200,000 daily bills, the interface-based design is suitable for maintainability: each extension implements a small contract and does not require editing working billing classes. Measurements show the per-bill virtual dispatch cost is small relative to the business logic in this run; the performance table should be remeasured on production hardware and with real allocation, I/O and payer workloads. For frequently rejected claims, expected validation errors should use a result/optional-style return rather than exceptions, as the 50% invalid benchmark made exceptions much slower.

## Plug-in alternative

Rates and simple package rules could be loaded from configuration and interpreted at runtime. With `r` package rules in an ordered map, finding a rule takes `O(log r)` per bill; a hash table gives expected `O(1)` lookup, plus rule parsing/evaluation time proportional to the rule size. A virtual call is `O(1)` dispatch. The configuration data requires `O(r)` space, with additional storage proportional to each rule's text. The plug-in design is a better fit for data-only changes; new behavior with validation or novel computations is safer in reviewed C++ classes. Configuration requires schema validation, versioning and protection against invalid rules.
