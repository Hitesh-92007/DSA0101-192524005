# Q1 — Constructors and constructor overloading

## Design and implementation

`Book` models the three library states: an empty record, a donated book with only a title, and a catalogued book with a title, author, and copy count. The default and title-only constructors delegate to the full constructor, so the validation and member initialization live in one place. All constructors use a member-initializer list. A negative copy count throws `std::invalid_argument`, and the run record includes that rejected input.

The class members are declared in the order `title_`, `author_`, `copies_`. C++ initializes them in that declaration order even if a mem-initializer list is written in another order. `ReorderProbe` deliberately lists its initializers in the opposite order: the compiler emits `-Wreorder`, while its printed values remain deterministic because the initializers are constants. The separate `q1_no_default_array.cpp` is expected not to compile: once a class declares a non-default constructor and supplies no default constructor, `Book books[2]` cannot default-construct its elements.

## Constructor and design answers

- A compiler supplies a default constructor only when the class declares no constructor of its own. Declaring a parameterized or overloaded constructor suppresses that implicit default constructor.
- Member-initializer lists initialize members directly. Assignment inside the constructor body would first default-construct class-type members and then assign to them; direct initialization is required for `const` and reference members and is generally clearer and more efficient.
- Member initialization order is declaration order, not the order written after the colon.
- After construction, the default object is `("", "", 0)`, the donated object is `("The Pragmatic Programmer", "Unknown", 0)`, and the catalogued object is `("Clean Code", "Robert C. Martin", 3)`.
- `explicit` is appropriate when a one-argument constructor should not silently convert an unrelated value into a `Book`-like object. `q1_explicit_probe.cpp` compiles without the keyword; compiling the same file with `-DEXPLICIT_CONSTRUCTOR` must fail at `PageCount pages = 300`.
- With a single constructor that has defaults, such as `Book(std::string title = "", int copies = 0)`, construction remains O(1) apart from string storage. The overloaded/delegating design is also O(1) in object fields and checks the copy count in one place. The default-argument form is shorter, but overloads make the three domain states explicit and reduce the chance that a caller supplies a title but unintentionally gets an invalid copy count.

## Experimental results

The run record demonstrates each constructor and the negative-copy rejection. The compile diagnostics are in `BUILD_RESULTS.md`: the explicit-conversion probe and the missing-default-constructor array are expected compile failures, while the reorder probe is an expected warning.

## Complexity

The integer and field setup is O(1); copying or moving the title and author takes time proportional to their lengths. The object stores O(1) scalar fields plus the strings' O(title length + author length) storage. A vector of default records requires O(number of records) objects.
