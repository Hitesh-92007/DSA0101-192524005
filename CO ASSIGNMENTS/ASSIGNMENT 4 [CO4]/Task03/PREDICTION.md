# Task 3 pre-run output prediction

## Complete the code

Pre-run prediction recorded before compiling `student_member_class_complete.cpp`:

```text
192311001 Kavya (19) Salem-636001
```

## Initializer-order experiment

Pre-run prediction: the compiler should report a `-Wreorder` warning because `second_` is listed before `first_`, even though `first_` is declared first; values should print in declaration order:

```text
1 2
```

## Construction trace

Pre-run prediction: each complete object constructs its virtual `Person` base before the other bases and members, then run the most-derived constructor. Destruction should reverse completion order. For `TeachingAssistant`, the trace should show one `Person`, followed by `Course`, `Student`, `Staff`, and `TeachingAssistant`; destruction should start with `TeachingAssistant` and finish with `Person` and its `Address` member. The Student trace will include registration number 192311001.

## Given Engine, Vehicle and Car program

Pre-run prediction recorded before compiling the assignment's `Engine`, `Vehicle` and `Car` program:

```text
Vehicle Engine Car | ~Car ~Engine ~Vehicle
```

The base `Vehicle` is constructed first, then the `Engine` member, then the `Car` constructor body. Destruction runs in reverse: `Car`, `Engine`, then `Vehicle`.
