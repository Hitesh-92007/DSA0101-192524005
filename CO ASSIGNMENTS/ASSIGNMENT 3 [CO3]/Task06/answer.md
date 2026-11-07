# Q6 — The rules of operator overloading

## Design and implementation

`Text` owns a null-terminated character array. It implements construction, deep copy construction, safe copy assignment, destruction, concatenation with `+`, equality and inequality, and stream insertion. Copy assignment handles self-assignment and allocates the replacement before releasing the old storage. Concatenation allocates one exact-sized result buffer and copies both operands once. Long and empty messages are exercised.

The three ownership functions form the Rule of Three: if a class manually owns a resource and defines any one of destructor, copy constructor, or copy assignment, it generally needs the other two to preserve correct copy semantics. A `std::string` member would follow the Rule of Zero and remove the need to hand-write these functions.

## Design answers

- Operators that cannot be overloaded include scope resolution `::`, member access `.`, pointer-to-member `.*`, conditional `?:`, and `sizeof`. Their syntax or compile-time meaning is part of the language grammar and cannot be changed by a class.
- Overloading cannot change precedence, associativity, or arity. At least one operand must be a user-defined type. The overload must preserve a meaning users can reasonably infer from the operator.
- Assignment `operator=` must be a member because the language requires it to assign the left-hand class object and its member signature has one explicit right-hand operand.
- A non-member or friend is useful when the left operand is not the class (as for `std::ostream << Text`) or when symmetric conversions should work. Friendship should be limited to operators that need private representation access.
- In `a + b + c`, left associativity forms `(a + b) + c`. Under C++17, the prvalue results are constructed directly, so this implementation records no copy-constructor calls for the expression. The operation still allocates and copies characters for each concatenation.
- Compared with `std::string`, this educational `Text` class shows ownership and the Rule of Three. `std::string` is the production choice because it already handles allocation, capacity, exceptions, and many optimized operations. The benchmark is machine/compiler dependent.

## Complexity

For input lengths `n1`, `n2`, `n3`, concatenating `a+b+c` takes O(n1+n2+n3) time but creates an intermediate string of length n1+n2 before the final result. The implementation's peak live result storage is O(n1+n2+n3); repeated appending with `result = result + next` can perform quadratic total copying. Reserving capacity and appending, or using `std::string`, is usually preferable for repeated concatenation.

## Experimental investigation

`RUN_OUTPUT.txt` records deep-copy independence, self-assignment, empty and long messages, comparisons, the C++17 copy count for `a+b+c`, and a 100,000-iteration concatenation comparison with `std::string`. `SANITIZER_RESULTS.txt` contains the address-sanitizer run. The result timings are one local build and are not portable performance claims.
