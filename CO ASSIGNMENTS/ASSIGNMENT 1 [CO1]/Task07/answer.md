# Task 7 — Procedural and object-oriented versions

`procedural.cpp` stores the balance in a local variable and passes it by reference to the free function `deduct`. `object_oriented.cpp` stores the balance in the private member `balance_` of `SmartCard`; callers use `canTravel`, `deduct`, and `balance` rather than modifying it directly.

Both versions use the same rule: accept only a non-negative fare when `balance - fare >= 5000`, and change the balance only after that check succeeds. The run begins at 10000 paise and tries a 3600-paise fare; both print `Allowed; remaining paise = 6400`. Each check and deduction takes O(1) time and each card stores O(1) state.

Private state plus a single deduction method makes the OOP invariant harder to bypass: every caller must pass through the same check. With a public balance, any code could subtract too much or assign a negative value. A change to the minimum balance can be made in one class constant or policy function in either design, but the class makes that rule the natural responsibility of the card.

The procedural design is small and direct for one operation. The object-oriented design is safer as the system grows because balance state and behavior stay together. The output records are in `procedural_output.txt` and `object_oriented_output.txt`.
