# Task 2 pre-run output prediction

## Virtual-base program

Pre-run prediction recorded before compilation:

```text
Person Student Staff TA
```

The virtual `Person` base should be constructed once, before the direct bases `Student` and `Staff`; the `TeachingAssistant` constructor runs last. Compilation is expected to succeed without a diagnostic.

## Non-virtual comparison

Pre-run prediction recorded before compilation:

```text
Person Student Person Staff TA
```

The constructor-only program is expected to compile without a diagnostic because duplicate base subobjects are allowed. Accessing `ta.name` should be ambiguous; `person_access_ambiguity.cpp` checks that separate compile-time result.
