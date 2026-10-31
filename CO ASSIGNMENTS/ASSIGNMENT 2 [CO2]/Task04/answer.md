# Q4 - Savings account class with a transaction array

## Class design

`SavingsAccount` keeps its account number, balance, ten-entry transaction array, start index, and count private. Public functions provide deposits, withdrawals, and a mini-statement. The small `deposit()` and `balance()` functions are defined inside the class; `record()`, `withdraw()`, and `miniStatement()` are defined outside with `SavingsAccount::` scope resolution.

Keeping the array private protects the count and ring positions from being changed inconsistently. The program records twelve valid transactions and prints the most recent ten (transactions 3 through 12). Its final balance is 1,560. A rejected deposit or withdrawal does not change the balance or history.

## How old transactions are discarded

The chosen implementation is a circular fixed array. While fewer than ten entries exist, a new record is placed after the latest record. Once full, the new record replaces the oldest and the start index advances. Adding a transaction is O(1) time and the storage is O(10), fixed per object. The eleventh transaction replaces transaction 1 without shifting the other nine.

Shifting a full fixed array left before every insertion costs O(k) for k stored entries; it is easy to understand but makes each insertion slower as the history grows. A `std::deque` can use `push_back` and `pop_front`, with O(1) time at the ends on average and dynamically managed storage. For a strict limit of 50, changing the fixed capacity from 10 to 50 is the smallest code change, at the cost of a larger fixed array in every account. A circular buffer still gives O(1) insertions at either capacity.

## Evidence

The full source is `q4_savings_account.cpp`; the recorded run output is `RUN_OUTPUT.txt`. No user input is needed because the test transactions are listed directly in `main()`.
