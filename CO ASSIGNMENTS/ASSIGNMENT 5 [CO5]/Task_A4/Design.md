# Task A4 — Exception Strategy

## Error cases and response

| Condition | Exception | Detected at |
|---|---|---|
| Negative, NaN or infinite charge | `NegativeChargeError : BillingError` | Bill constructor / payer validation |
| Missing patient record | `MissingPatientError : BillingError` | `Bill` constructor |
| Unknown day-care code | `PackageError : BillingError` | `DayCareBill` constructor |
| Claim exceeds policy cover | `ClaimError : BillingError` | `settle()` / claim approval |
| Text is not a number | `std::invalid_argument` from `std::stod` | Input parsing |
| Invalid numeric range | `std::out_of_range` from `std::stod` | Input parsing |
| Allocation failure | `std::bad_alloc` (including `std::bad_array_new_length`) | Allocation |

## Hierarchy

```text
std::exception
├── std::runtime_error
│   └── BillingError
│       ├── NegativeChargeError
│       ├── MissingPatientError
│       ├── PackageError
│       └── ClaimError
├── std::invalid_argument
├── std::out_of_range
└── std::bad_alloc
```

Validation, constructors, map/vector growth, and settlement may throw. `total()`, `type()`, accessors, `dailyTotal()` and destructors are marked `noexcept` because they only read validated state or release resources. A destructor must not let an exception escape during stack unwinding; a second exception then terminates the program.

## Alternative: return status

An enum such as `Status::{ok, negative, aboveCover}` can represent expected input rejection. It adds a status branch to every caller and requires a separate output location or result structure for a message/value. Validation has a small fixed check cost on success and failure. Exceptions also have low normal-path cost in common implementations but are substantially more expensive when repeatedly thrown. Use return values for routine expected rejection and exceptions for exceptional failures that need to cross several layers; this benchmark compares both on claim validation.
