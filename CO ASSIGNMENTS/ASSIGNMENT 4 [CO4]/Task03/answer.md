# Task 3 Constructors and Member Classes

## Constructor initializer lists

The `Student` constructor initializes its virtual `Person` base, then its `Course` member and registration number. `Staff` initializes the same virtual base and its department. When `TeachingAssistant` is the most-derived type, it initializes the shared virtual `Person` base directly; the `Person` initializer inside the intermediate `Student` and `Staff` constructors is ignored for that object. This prevents the two branches from choosing conflicting values for the shared base.

## Member classes and traces

`Address` is stored by value in `Person`; `Course` is stored by value in `Student`. Their constructors run in the order of their containing object, and their destructors run after the containing class destructor. See `construction_trace.cpp` and `RUN_OUTPUT.txt` for the exact observed trace for Student, Staff and TeachingAssistant.

The general order is: virtual bases, direct bases in declaration order, members in declaration order, then the constructor body. Destruction reverses that order. A virtual-base `Person` is constructed once in the teaching-assistant object. The separate non-virtual comparison has two `Person` subobjects; `nonvirtual_size.cpp` prints sizes for this compiler and a one-byte Person payload so the virtual-base storage cost is visible. Exact byte counts can differ across compilers.

## Complete the code

The completed `Student` constructor is `: Person(n, a), regNo(r), home(c, p)`. It initializes the base and members in declaration order. The program prints the expected record `192311001 Kavya (19) Salem-636001`.

In `initializer_order_warning.cpp`, the initializer list deliberately lists `second_` before `first_`. The compiler warns because initialization still follows member declaration order, so this ordering can mislead readers and cause bugs when one member depends on another. The program prints `1 2`.

## Pointer alternative for Address

A dynamically allocated `Address*` adds one pointer field and a heap allocation per Student, and requires explicit ownership. A safe implementation should use `std::unique_ptr<Address>` rather than a raw owning pointer. If a raw owning pointer is used, the constructor must allocate, the destructor must release, and copying must deep-copy or be disabled; otherwise two Student objects can share one pointer and double-delete it. A value member needs no separate allocation and follows the containing object's lifetime automatically.

## Predict the supplied output

The pre-run prediction is recorded in `PREDICTION.md`. The added program copied from the assignment printed `Vehicle Engine Car | ~Car ~Engine ~Vehicle`. The prediction matched the run. A base subobject is constructed before the derived object, then its member is constructed before the derived constructor body; destruction runs in reverse order.
