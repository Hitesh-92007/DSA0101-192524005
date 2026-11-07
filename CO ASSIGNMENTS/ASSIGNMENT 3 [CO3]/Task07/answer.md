# Q7 — Design a complex-number class from first principles

## Stage 1 — Interpret the problem

The state is a pair of `double` values: real and imaginary parts. Required operations are addition, subtraction, multiplication, division, equality, rectangular output, and magnitude. The class needs a default constructor, a one-argument real constructor, and a two-argument constructor. Measurable checks are: operator results agree with hand calculations and `std::complex<double>` within floating-point precision; a zero divisor is rejected; and series and parallel RLC impedances are printed in rectangular form with magnitude.

## Stage 2 — Design

For `a = x + jy` and `b = u + jv`:

- `a + b = (x+u) + j(y+v)`
- `a - b = (x-u) + j(y-v)`
- `a * b = (xu-yv) + j(xv+yu)`
- `a / b = ((xu+yv) + j(yu-xv)) / (u²+v²)`; if `u²+v²` is zero, throw `std::domain_error`.
- `|a| = hypot(x,y)`.

The arithmetic and equality operators are members; stream insertion is a friend non-member because the stream is the left operand. Each operation is O(1) time and O(1) extra space; the object stores two doubles.

## Stage 3 — Implement and verify

The program computes a series RLC impedance and a parallel RLC impedance for R=10 Ω, L=0.1 H, C=100 μF, and f=50 Hz. It verifies the series result against `std::complex<double>`, prints the magnitudes, and catches division by zero.

Hand check: `ω = 2πf ≈ 314.1593 rad/s`, so `ZL = jωL ≈ j31.4159 Ω` and `ZC = -j/(ωC) ≈ -j31.8310 Ω`. The series result is therefore approximately `10.0000 - j0.4151 Ω`, with magnitude about `10.0086 Ω`. The parallel branches use `(Z1×Z2)/(Z1+Z2)` twice. The result is near resonance and close to the 10 Ω shunt-resistor limit. The exact rounded values are in `RUN_OUTPUT.txt`.

## Stage 4 — Defend the design

`std::complex<double>` already supplies a tested complex-number implementation, including arithmetic and magnitude. This class is useful here to demonstrate how operator formulas map to code, but production circuit software should normally use the standard library. One limitation is the exact zero check in the denominator: floating-point calculations close to zero may still produce very large results. A real simulator should also define a tolerance or a domain-specific singularity policy. Floating-point equality is exact component equality in this example and is therefore suitable only for the controlled tests shown.

## Alternative and complexity

Replacing this class with `std::complex<double>` leaves each operation O(1) time and two-double O(1) space, while reducing custom code and relying on the standard implementation. Division-by-zero policy must still be checked by the application if it requires an exception or diagnostic; the standard type does not apply this class's explicit exception policy.
