# Q1 - Call by value, reference and pointer

## Prediction for the supplied program

Before running the supplied code, the expected lines are:

```text
1 12 13
1 100
16 13 6
```

`a` is copied into parameter `a`, so its caller value remains 1. `b` is an alias for the caller's `b` and becomes 12. `c` is reached through the pointer and becomes 13. `bigger(a, c)` returns a reference to the larger object, `c`, so assigning 100 changes `c`. The defaults for `show(1)` are `y=5` and `z=10`; `show(1, 2)` uses only `z=10`; the final call supplies all three values.

## Fee update methods

The program runs the three parameter styles on separate copies of a 45,000 fee:

| Style | Example | Stored fee after call | Reason |
|---|---|---:|---|
| Value | `applyByValue(fee, 10)` | 45,000 | The parameter is a local copy. |
| Reference | `applyConcession(fee, 15)` | 38,250 | The reference names the caller's object. |
| Pointer | `applyByPointer(&fee, 25)` | 33,750 | Dereferencing the pointer writes to the caller's object. |

The completed TODO applies 10% by reference. Integer arithmetic computes `fee * percent / 100`; on 45,000 the result is 4,500, leaving 40,500. The actual run is recorded in `RUN_OUTPUT.txt`.

## Interface decision and complexity

For a function whose stated job is to update an existing student record, a non-const reference makes the mutation visible in the parameter list and avoids copying the record. A pointer is useful when “no object” is a valid input, but then it must be checked for null. For combining several concessions, returning a new fee is also a good interface: calculate each step and assign the final value explicitly. The returned-value version and the in-place version both take O(1) time for a fixed number of percentage operations.

On this compiler target, `sizeof(long long)` is 8 bytes. Passing it by value copies those 8 bytes; passing by reference or `const` reference copies no fee object and uses an implementation-sized reference. Passing a large `Student` by value copies all of its fields and padding, so its copy cost grows with the object size. Passing it by `const Student&` avoids that copy while preventing changes through that parameter. The reference and pointer versions still require the caller's object to remain alive for the call.

## Reference implementation

`q1_call_modes.cpp` includes the supplied output-prediction program and the completed concession examples. The output file is an actual run transcript from this workspace; make your own prediction before running it independently for the assignment requirement.
