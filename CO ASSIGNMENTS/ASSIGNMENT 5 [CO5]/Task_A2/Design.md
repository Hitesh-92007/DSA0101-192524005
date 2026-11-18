# Task A2 — Runtime Binding and Destruction

## Early and late binding

An ordinary non-virtual call is selected from the static type at compile time. A virtual call such as `Bill* b = ...; b->total();` is dispatched at runtime using the object's dynamic type. In the supplied example, `show()` itself is non-virtual, but it calls the virtual `total()`, so the call reaches `OPBill::total()` and prints `500`.

## Conceptual vtables

The accompanying `Vtable_Binding.png` diagram traces `Bill*` to a derived table and its `total()` slot.

| Class | `total()` entry | Destructor entry |
|---|---|---|
| `Bill` | pure virtual (abstract) | `Bill::~Bill` |
| `OutPatientBill` | `OutPatientBill::total` | `OutPatientBill::~OutPatientBill`, then base destructor |
| `InPatientBill` | `InPatientBill::total` | `InPatientBill::~InPatientBill`, then base destructor |

A `Bill*` points to an object whose vptr refers to the derived class table. The `total()` slot selects the derived function. This is a conceptual model; C++ does not require a particular vtable layout.

## Prediction and execution

Predicted from the code: `500 ~OPBill ~Bill ` followed by a newline. The saved run output is the same. `show()` calls virtual `total()`; deleting the pointer calls the derived destructor first and then `Bill::~Bill`.

After removing `virtual` from the base destructor, deleting an `OPBill` through `Bill*` has undefined behavior. The compiler warns about the non-virtual base destructor. This toolchain printed `500 ~Bill finished`; it skipped the derived destructor. AddressSanitizer did not report the defect in this environment, and LeakSanitizer is unsupported here, so that output is not evidence of safety.

## Alternative: `dynamic_cast`

Without virtual `total()`, code could test each possible derived type with `dynamic_cast` and call the corresponding formula. For `k` types, a bill could require up to `k` casts, compared with one virtual dispatch. A polymorphic base also stores implementation-specific runtime type data; typical implementations use a vptr per object. The casts create a growing chain of central type checks and need changes when adding each class.
