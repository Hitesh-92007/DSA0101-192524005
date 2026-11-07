# Q5 — Increment, comparison, and subscript operators

## Design and implementation

`Time` stores seconds since midnight. Prefix `++` advances in place and returns `Time&`; postfix `++(int)` saves and returns the old value. Both wrap at 24 hours. `==` and `<` compare the normalized second count. `Playlist` stores tracks in `std::vector<std::string>` and provides checked mutable and const `operator[]` overloads. Invalid indices throw `std::out_of_range`, which avoids returning a dangling or meaningless reference.

## Prediction question

Before execution, the `Counter` example evaluates as follows: `a = c++` gives `a=5` and then `c=6`; `b = ++c` increments first and gives `b=7`, leaving `c=7`. Therefore its output is `5 7 7`. The saved run confirms this reference prediction.

## Design answers

- The postfix signature has an unused `int` parameter solely to distinguish it from prefix `++()`; callers write `x++` and do not supply that parameter.
- Postfix must return the old value, so it creates a copy before changing the object. Prefix returns the changed object by reference and avoids that value copy. For simple built-in integers both are usually cheap; for class types prefix is commonly preferred when the old value is not needed.
- `operator[]` needs both const and non-const forms so read-only playlists can be indexed without allowing mutation, while ordinary playlists can update tracks. This implementation checks bounds and throws; unchecked built-in array access has undefined behavior, while `std::vector::at` also throws on an invalid index.
- `playlist[-1]` and `playlist[size]` are both rejected with `std::out_of_range`. The additional comparison is O(1); the access remains O(1).
- At minute 59 the next second rolls the minute. At 23:59:59 this implementation wraps to 00:00:00, using a 24-hour clock.

## Experimental investigation

The run record includes the prefix/postfix values, minute and hour rollover, mutable and const playlist access, and rejected indices. It measures 10,000,000 prefix and postfix increments. The saved benchmark is specific to this Windows LLVM-MinGW build and machine; the postfix result depends on whether the returned copy is consumed and how the compiler optimizes it. `BENCHMARK_RUNS.txt` includes five runs at each optimization level (`-O0` and `-O2`).

## Complexity

Increment, comparison, and checked indexed access are O(1) time and O(1) extra space. A playlist stores O(number of tracks + total track-name characters) space.
