# Task B1 — Billing Implementation and Validation

## Hand-checked bill totals

| Bill | Calculation | Total |
|---|---|---:|
| OP Ravi | 500 + 1,200 | 1,700.00 |
| IP Meena | 3 × 2,500 + 40,000 | 47,500.00 |
| Day-care Arun | fixed package DC-101 | 32,000.00 |
| Emergency Nila | 2,500 + 1,200 × 2 hours | 4,900.00 |
| All four | 1,700 + 47,500 + 32,000 + 4,900 | 86,100.00 |

## Payer result verification

Insurance uses a 20% co-payment and a 50,000 cover limit. Government rates are OP 1,500, IP 40,000, day-care 30,000 and emergency 3,500. `patient_due = bill_total - covered`.

| Bill | Payer | Covered | Patient due |
|---|---|---:|---:|
| OP | SelfPay | 0.00 | 1,700.00 |
| OP | Insurance | 1,360.00 | 340.00 |
| OP | GovernmentScheme | 1,500.00 | 200.00 |
| IP | SelfPay | 0.00 | 47,500.00 |
| IP | Insurance | 38,000.00 | 9,500.00 |
| IP | GovernmentScheme | 40,000.00 | 7,500.00 |
| Day-care | SelfPay | 0.00 | 32,000.00 |
| Day-care | Insurance | 25,600.00 | 6,400.00 |
| Day-care | GovernmentScheme | 30,000.00 | 2,000.00 |
| Emergency | SelfPay | 0.00 | 4,900.00 |
| Emergency | Insurance | 3,920.00 | 980.00 |
| Emergency | GovernmentScheme | 3,500.00 | 1,400.00 |

The program output in `Output_Screenshots/billing_complete.txt` matches these calculations. The separate `complete_code.cpp` program reproduces the supplied output exactly:

```text
OP Ravi: 1700
IP Meena: 47500
Total 49200
```

Removing `virtual` while retaining `= 0` fails to compile: the compiler says `total` is not virtual and cannot be declared pure. A non-virtual ordinary implementation would compile but base-pointer calls would no longer dispatch to the derived calculation.

## Alternative: templates

Separate `std::vector<OutPatientBill>`, `std::vector<InPatientBill>` and other typed containers can use templates and direct calls with no virtual dispatch. Total computation remains `O(n)` and each object avoids a base pointer. The trade-off is that one heterogeneous ordered bill list needs a variant, separate traversal/merge logic, or another type-erasure mechanism. The polymorphic vector of owning base pointers gives a simple shared collection and supports extension through the stable interface.
