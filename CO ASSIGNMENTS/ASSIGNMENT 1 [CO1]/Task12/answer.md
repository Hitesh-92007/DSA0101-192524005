# Task 12 — Re-evaluate and adapt the design

## Impact analysis

Airport requires a fourth `Zone` value and a Rs 60 base fare (6000 paise). The fare table and any validation range must grow from three to four entries. If the code stayed switch-based, every relevant switch would need an Airport case. The weekend rule changes fare selection: a 10% discount applies on weekends and does not combine with the peak surcharge. A `MonthlyPass` must store two chosen zones and report whether it covers the requested zone. `SmartCard` still owns the balance and minimum-balance invariant.

## Evaluation and redesign

The original `SmartCard` remains useful because it centralizes payment validation. A switch can be extended with Airport, but a table is easier to maintain as zones grow. I therefore **modify** the design: use a four-entry fare array indexed only after validating the enum; evaluate pass coverage first; otherwise apply either the weekend discount or the weekday peak surcharge. The rules are mutually exclusive, so a weekend peak trip receives the weekend discount only.

Worked examples: Airport weekday peak is 6000 × 120% = 7200 paise (Rs 72). Airport on a weekend during peak time is 6000 × 90% = 5400 paise (Rs 54), with no extra peak surcharge. An Airport trip covered by a valid monthly pass costs 0 paise. The run output shows all three values; the covered pass trip is accepted and leaves the 20000-paise balance unchanged.

`redesigned_fare.cpp` implements the fare table, weekend/weekday choice, two-zone pass, and private card balance. The pass accepts only two distinct valid zones; an invalid selection covers no zone. Zone lookup and pass checks are O(1); storing the selected zones takes O(1) space. The test examples support the redesign: each new rule has a distinct worked case. The trade-off is that a table relies on a validated mapping between enum values and indexes, while a switch is more explicit but requires editing each switch for every new zone.

Before final deployment, define pass expiry and renewal behavior and confirm the student/senior concession policy. The example code covers the assignment's three new requirements but does not implement those unspecified business rules.
