# Task 4 pre-run output prediction

For the supplied virtual/non-virtual rate program, pre-run prediction:

```text
3.5 4 3.5
```

`rate()` is non-virtual, so `p->rate()` is selected using the static type `Account*` and returns 3.5. `vrate()` is virtual, so the same pointer dispatches to `SeniorAccount::vrate()` and returns 4.0. `copy` is an `Account` value created from a `SeniorAccount`, so the derived part is sliced away and its virtual call uses `Account::vrate()`, returning 3.5.

For `account_fixed.cpp`, the expected rates are 3.5 and 4.0, and the senior destructor should run before the base destructor. The defective program is expected to show the base rate twice and leave one tracked history buffer outstanding after deleting the accounts, because the derived destructor is skipped through the non-virtual base pointer. AddressSanitizer wording depends on the platform runtime.
