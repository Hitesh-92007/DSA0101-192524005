# Task 6 — Control structures and pseudocode

## Fare algorithm

```text
ALGORITHM ProcessJourneys
    FOR each journey in the input list
        IF zone is not Inner, Middle, or Outer OR hour is outside 0..23 THEN
            report invalid journey
            CONTINUE
        END IF

        SWITCH zone
            CASE Inner: basePaise <- 1000
            CASE Middle: basePaise <- 2000
            CASE Outer: basePaise <- 3000
            DEFAULT: report invalid zone; CONTINUE
        END SWITCH

        peak <- (8 <= hour AND hour < 10) OR (17 <= hour AND hour < 20)
        farePaise <- basePaise
        IF peak THEN
            farePaise <- farePaise + farePaise * 20 / 100
        END IF

        IF cardBalancePaise - farePaise >= 5000 THEN
            cardBalancePaise <- cardBalancePaise - farePaise
            print fare and new balance
        ELSE
            print insufficient balance; leave balance unchanged
        END IF
    END FOR
END ALGORITHM
```

This pseudocode uses integer paise. The program determines peak status from the hour itself. The 10:00 and 20:00 boundaries are off-peak because each peak interval excludes its end. The supplied Task06 code template asks for zone fare, peak surcharge, and minimum balance; it does not provide a concession input. The scenario gives no concession percentage, so that separate rule remains an assumption to confirm before implementation.

## Control-structure choices

Use `switch` for the small, fixed `enum class Zone` set and include a `default` to reject invalid values. Use `if` for hour ranges, input validation, and the balance invariant. A conditional operator is suitable for a short selection but is less readable for multiple fare rules. A range-based `for` loop is best for processing a known container of journeys; an indexed `for` loop is useful when the index matters; `while` is appropriate when repeatedly prompting until input is valid. `continue` skips a bad journey; `break` can stop processing when input ends or a fatal error occurs.

Both a switch and a checked fare-array lookup take O(1) time. A `constexpr` fare array uses a small fixed amount of storage and makes adding Airport a table change; a switch gives an explicit branch and needs a new case. Processing `n` journeys is O(n) time and each card's stored state is O(1).

## Run evidence

`fare_complete.cpp` preserves the three sample outcomes: Rs 10.00 leaves Rs 90, Rs 36.00 leaves Rs 54, and a later Rs 20 fare is denied without changing the balance. It also runs two separate boundary journeys: Inner at exactly 10:00 costs Rs 10.00, and Outer at exactly 20:00 costs Rs 30.00. Both are reported as off-peak. Task09 independently checks all four peak boundaries.
