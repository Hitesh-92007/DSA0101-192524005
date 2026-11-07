# Q4 — Overloading arithmetic and stream operators

## Design and implementation

`Money` stores one integer count of paise. This keeps addition, subtraction, equality, and ordering exact. The class overloads `+`, `-`, `+=`, `==`, and `<`; stream insertion prints a stable rupee-and-paise representation, and extraction parses a decimal token without converting through binary floating point. The billing calculation reads naturally as `subtotal = a + b`, and repeated additions can use `+=`.

## Design answers

- `+` returns a new value and leaves both operands unchanged. `+=` updates the left operand and returns `Money&` to `*this`, allowing chaining without a temporary result.
- Arithmetic and comparisons are members because they inspect the stored paise value; with the chosen implicit one-argument rupee constructor, the member form supports `money + rupees`. A non-member friend `operator+(Money lhs, const Money& rhs)` would allow implicit conversion on either side, so both `money + 100` and `100 + money` could work. Stream operators are non-members because the left operand is an `std::ostream`/`std::istream`, not a `Money`; they are friends so they can access the private representation.
- `a + b` is translated to `a.operator+(b)` for this member overload. For a non-member operator it would be `operator+(a, b)`.
- `<<` must be a non-member so an output stream can appear on the left (`std::cout << amount`). The function returns the stream to support chaining.
- Multiplication of two money amounts is usually not meaningful: it produces squared currency units. A domain-specific operation such as applying a tax rate should use a scalar/rate and define a clear rounding rule.

## Experimental investigation

The run record includes paise carry (`Rs 0.75 + Rs 0.50`), negative subtraction, parsing `12.34`, `+=`, ordering, and equality. It also records one million additions using integer paise and `double`. Both loops have O(1) work per iteration and O(1) extra space. Integer paise is exact to a paisa; repeated binary floating-point increments can accumulate representation error.

## Complexity

Each arithmetic or comparison operation takes O(1) time and O(1) additional space. The stream operators take O(number of printed or parsed digits). The internal state always occupies one `long long`.
