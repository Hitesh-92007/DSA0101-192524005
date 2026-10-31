# Q3 - Default arguments and function overloading

## Why the original set is ambiguous

With `charge(double)`, `charge(double, bool)`, and `charge(double, bool = false, bool = false)`, a one-argument call such as `charge(2.0)` can match the first function and the third function (using its default values). Neither candidate is a better match, so the compiler reports an ambiguous call. Default arguments do not create a distinct function signature.

## Redesigned interface and six calls

`q3_courier_overloads.cpp` uses one `int` overload for grams, one `double` overload for kilograms, and one `Parcel` overload. Optional flags are defaulted only on the numeric overloads. A `Parcel` is passed by `const` reference to avoid copying it. The six calls in the program select:

| Call | Selected function | Reason |
|---|---|---|
| `charge(2)` | `charge(int, bool, bool)` | Exact match for an integer literal. |
| `charge(2.0f)` | `charge(double, bool, bool)` | `float` to `double` is a promotion; conversion to `int` is worse. |
| `charge(2.0)` | `charge(double, bool, bool)` | Exact match for a double literal. |
| `charge(750, true)` | integer grams overload | Exact match for the first argument; the last option uses its default. |
| `charge(parcel)` | `charge(const Parcel&)` | Exact match for the object type. |
| `charge(2.5, false, true)` | kilogram overload | Exact match for the double first argument. |

The output labels the selected overload and displays its computed charge. The predictions above are reference reasoning; write your own predictions before running the program independently.

## Overload rules and alternatives

The compiler considers viable overloads and prefers an exact match over a promotion, and a promotion over a standard conversion. If two viable candidates have equally good conversions, the call is ambiguous. Default arguments must be supplied from the right because a call may omit only trailing arguments; a required argument cannot follow an omitted one.

Overload resolution happens at compile time and adds no run-time dispatch cost. A single `Parcel` structure with optional fields would make adding options explicit in one object and avoid overload growth, but makes even simple calls longer. A function template could generalize input types, but needs constraints and still requires clear rules for units. Passing a parcel by `const` reference avoids copying its fields; passing by value copies them. The numeric arguments are small scalars. Each charge calculation takes O(1) time.
