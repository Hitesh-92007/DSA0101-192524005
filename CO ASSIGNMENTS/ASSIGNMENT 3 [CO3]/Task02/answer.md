# Q2 — Copy constructors: shallow and deep copies

## Design and implementation

`Prescription` owns a dynamically allocated array of medicine names. The correct version implements the Rule of Three: the destructor releases the array, the copy constructor allocates and copies a separate array, and copy assignment makes a replacement before releasing the old array. The replacement-first order gives the assignment operator a strong exception guarantee. Self-assignment returns immediately. Index access is checked.

The source can also be compiled with `-DSHALLOW_VERSION`. That deliberately copies only the pointer, so both objects alias the same array and their destructors attempt to delete it. AddressSanitizer reports the resulting double-free. The correct build reports independent contents after modifying the original, successful self-assignment, and one copy for by-value passing versus zero for a `const` reference.

## Design answers

- A shallow copy makes both objects point to one array. Updating either object's medicine changes the shared element. If both destructors delete the pointer, the program attempts a double delete; this is undefined behavior and is commonly diagnosed by AddressSanitizer.
- A deep copy allocates `n` new strings and copies the `n` values. The two objects then have independent ownership.
- The Rule of Three applies when a class manually manages a resource and needs any one of destructor, copy constructor, or copy assignment: it generally needs all three. Defining only one leaves the compiler-generated copies with unsafe pointer semantics. Modern C++ recommends the Rule of Zero with standard containers such as `std::vector<std::string>` when raw ownership is not itself the learning objective.
- Copy construction occurs when a new object is initialized from another (including pass-by-value and some returns); copy assignment occurs when an already-created object is assigned. C++17 can elide some temporary copies.
- Deep copy construction and assignment are O(n) in the number and total character length of medicine names, and use O(n) additional array space. A shallow pointer copy is O(1) time and space but violates independent ownership.
- Replacing the raw array with `std::vector<std::string>` uses compiler-generated memberwise copies that copy the vector's elements deeply. Complexity remains O(n) time and O(n) storage; the container manages lifetime and exceptions.

## Experimental investigation

`RUN_OUTPUT.txt` records independent-copy and self-assignment checks, copy counts for by-value versus `const` reference, and timings for 10, 1,000, and 100,000 medicine entries. The source provides the shallow-ownership version for the sanitizer run and the Rule-of-Three version for the correct run. Sanitizer diagnostics are stored separately in `SANITIZER_RESULTS.txt` so the expected shallow failure is not confused with the clean run.

## Conclusion

Rely on the generated copy constructor when members already manage their own values correctly (for example `std::string` and `std::vector`). Write a custom copy constructor when the class itself owns a resource whose default memberwise copy would alias ownership, such as a raw owning pointer.
