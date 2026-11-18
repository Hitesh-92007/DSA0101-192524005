# Task A3 — Abstract Payer Interface

`Payer` is an abstract interface with `amountCovered(const Bill&)` and `name()`. Each policy stores its own data and implements coverage without changing `Bill` or the settlement routine.

| Payer | Coverage calculation |
|---|---|
| SelfPay | `0` |
| Insurance | `min(coverLimit, billTotal × (100 − copayPercent) / 100)` |
| GovernmentScheme | `min(billTotal, fixedRate[bill.type()])` |
| CorporateTieUp extension | `min(corporateLimit, billTotal × coveragePercent / 100)` |

**Algorithm 2: SETTLE_BILL**

```text
BEGIN SETTLE_BILL(bill, payer)
    SET total <- bill.total()
    SET covered <- payer.amountCovered(bill)
    IF covered < 0 OR covered > total THEN
        THROW ClaimError
    END IF
    RETURN (total, covered, total - covered)
END
```

Each settlement takes `O(1)` time for the implemented payer formulas (the government implementation performs a map lookup, `O(log r)` for `r` package rates). The returned settlement uses `O(1)` additional space. `CorporateTieUp` in Task B4 demonstrates extension without modifying existing classes.

## Alternative: callable policy

A `std::function<double(const Bill&)>` could hold a policy callable and capture the cover limit or percentage. Its invocation is also `O(1)` in the abstract model, but policy data is stored in the callable's closure (or external object) instead of named payer fields. Type erasure can have a small runtime/space cost. The interface class makes policy identity, state and validation explicit.
