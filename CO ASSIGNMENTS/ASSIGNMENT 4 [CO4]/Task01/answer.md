# Task 1 Class Design Review

## 1.1 Design deficiencies

1. `name`, `id`, and parts of the pay logic are repeated across the classes.
2. All fields are public, so callers can put an object into an invalid state.
3. The same concepts use different names: `name`/`nm` and `id`/`code`.
4. `da` is stored but ignored by `pay()`.
5. `0.42` is a magic number repeated inside a formula rather than a named policy constant.
6. Each staff category has a different method signature and cannot be processed through one common interface.
7. Adding a category requires duplicating common details and updating client-side selection logic.

## 1.2 Repaired hierarchy

`Staff` owns the common name and ID and declares the abstract `pay()` operation. `Professor`, `LabAssistant`, and `GuestFaculty` keep only the data used by their own pay formula. The members are private, and each override has the same interface. The program stores owning base-class pointers in one payroll collection and calls `pay()` polymorphically.

```text
                 Staff (abstract)
          name, id; virtual pay() = 0
             /          |          \
     Professor    LabAssistant   GuestFaculty
     basic,        basic           hours, rate
     research
```

The matching image is `UML.png`; the compiled implementation is `payroll_hierarchy.cpp`.

## 1.3 Design defence (under 100 words)

The common `Staff` interface keeps name and ID in one place and requires every category to define its own pay rule. The payroll loop can handle all categories through `Staff` pointers, so the caller does not need a type switch. To add another category, I can derive a class and implement `pay()` while leaving the current classes and loop unchanged. Private fields protect each object from unrelated code, and the named DA rate makes the shared rule visible and easy to revise.

## Alternative: one class with a type field

One `Staff` class could store a category code, all fields, and a `pay()` switch. A call still takes O(1), as does virtual dispatch in this example. The single object has to reserve fields for research allowance, hours, and rate even when a particular staff member does not use them. Adding a type usually changes the enum, the class storage or constructor validation, the `pay()` switch, and often client output/creation logic. In the hierarchy, adding a derived class and its creation path is sufficient when the caller already uses the common interface.

## Sample run

The program calculates pay as professor 81,000.00, lab assistant 42,600.00, and guest faculty 30,000.00, using the supplied sample values.
