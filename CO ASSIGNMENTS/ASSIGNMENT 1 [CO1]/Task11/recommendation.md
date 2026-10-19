# Final recommendation

Use `enum class Zone`, a centralized fare table, `long long` paise, validated input, and a `SmartCard` with a private balance and checked payment methods. This keeps fare lookup and card authorization O(1) per journey and handles millions of journeys in O(n) total time. Task09 verifies peak boundaries, invalid cases, exact Rs 50 remaining, and a five-trip total; Task10 shows why `int` is too small for network paise totals and why `double` accumulation is approximate; Task12 demonstrates Airport, weekend pricing, and passes.

See [`answer.md`](answer.md) for the full justification, configuration-file alternative, limitations, and assumptions about the unspecified student/senior concession.
