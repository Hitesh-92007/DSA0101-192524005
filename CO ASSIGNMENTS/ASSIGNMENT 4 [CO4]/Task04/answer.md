# Task 4 Pointers to Derived Classes Forensics

## Defects

1. `Account::rate()` is non-virtual, so calls through `Account*` bind to the base implementation and the senior rate is never selected.
2. `Account` has a non-virtual destructor. Deleting a `SeniorAccount` through `Account*` is undefined behavior; the derived destructor may not run, leaving `history` allocated.
3. `Account copy = SeniorAccount()` copies only the base subobject. The derived state and dynamic type are lost by object slicing.
4. The raw `accounts` pointers require manual ownership and make the unsafe deletion path easy to repeat.
5. `rate()` in the derived class lacks `override`, so a signature mismatch would not be diagnosed as an override error.

## Sanitizer evidence and repair

`account_defects.cpp` retains the defective ownership, adds destructor messages, and tracks live history buffers. The build script compiles it with AddressSanitizer and records the run in `SANITIZER_RESULTS.txt`. This Windows AddressSanitizer runtime does not support leak detection, so the output also shows that one history buffer remains after deletion and that the derived destructor message is absent. Exact sanitizer diagnostics vary by runtime.

`account_fixed.cpp` makes the base destructor virtual, declares the rate as virtual and uses `override`, stores objects in `std::unique_ptr<Account>`, and keeps the dynamic type behind the pointer instead of copying into `Account` values. Its output shows 3.5 and 4.0 and releases the derived vector automatically.

Two prevention rules are: give polymorphic bases a virtual destructor, and mark intended overrides with `override`. Use owning smart pointers and avoid storing polymorphic derived values by value.

## Predict the output

The prediction and actual program output are documented in `PREDICTION.md` and `RUN_OUTPUT.txt`. The three calls produce `3.5 4 3.5` for the static dispatch, dynamic dispatch, and sliced base object respectively.

## `std::unique_ptr` comparison

Processing n accounts remains O(n) time, with O(n) pointer storage. Compared with raw owning pointers, `unique_ptr` still uses one pointer per account but automatically destroys the derived object through the virtual base destructor. It does not make a non-virtual base destructor safe, and it does not prevent slicing if code still copies a derived object into an `Account` value. The virtual call and ownership model must both be designed correctly.
