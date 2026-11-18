# Task B2 — Engineered Error Conditions

## Prediction from the supplied program

Before checking the saved output, the control flow predicts:

```text
ok done caught negative [log too large] caught too large 
```

The `100` case validates and prints `ok `, then `done `. The `-5` case throws `invalid_argument`, which is not caught by `process`'s `out_of_range` handler; the top-level `std::exception` handler prints `caught negative `. The `2e6` case throws `out_of_range`; `process` logs it and uses `throw;`, then the top-level handler prints `caught too large `. Only the `out_of_range` case is logged by `process`.

The executed `predict_output.cpp` output matches this prediction.

## Error tests

| Test | Expected response | Observed result |
|---|---|---|
| Negative charge | Reject before constructing a bill | `NegativeChargeError` message caught; program proceeds |
| Claim 60,000 with 50,000 cover | Log once and rethrow | `audit: exceeds cover`, then typed rejection |
| Unknown day-care code | Reject unknown package | `PackageError: unknown package code: NO-SUCH-CODE` |
| `std::stod("not-a-number")` | Standard parsing exception | `invalid_argument` caught as `std::exception` |
| Impossible array size | Allocation failure catch path | Normal build catches `std::bad_alloc` / `std::bad_array_new_length` |
| RAII objects then throw | Destructors run during unwind | `audit-file` destroyed before `patient-record`; top-level catch continues |

Under AddressSanitizer, the impossible size is rejected by ASan before the C++ exception can be caught. The ASan build therefore injects `std::bad_alloc` in that one case to test the catch/recovery path. This distinction is called out in the run record. Other error cases in the ASan suite are executed normally.

## Completed claim code output

`complete_claim_code.cpp` matches the PDF's expected four lines:

```text
approved 40000
audit: exceeds cover
claim rejected: exceeds cover
error: negative amount
```

`throw;` rethrows the same active `ClaimError`, retaining its dynamic type. `throw e;` creates a new exception from the static type of `e`; if `e` referred to a derived exception through a base-class reference, the new exception could be sliced.
