# Q2 - Return by reference and its dangers

## What the program demonstrates

`highestMark` scans the array and returns a reference to its largest element. The caller can read it or write through it: `int& top = highestMark(...); top += 2;`. This changes the array element from 91 to 93. A `const int&` can read the value without copying it but cannot change it through that name. A returned `int` copy is independent; changing the copy does not update the array.

The run also shows the index and pointer alternatives. The index is used as `marks[index]`; the pointer is used as `*pointer`. Each search is O(n) time and O(1) extra space. Returning an index is often the clearest choice if the caller needs to refer back to the array later. A pointer has the same lifetime requirement as a reference.

## Dangling reference warning

`danglingReference()` is included solely to trigger the compiler diagnostic and is never called. It returns `localMark`, whose lifetime ends when the function returns. The compiler reports `reference to stack memory associated with local variable 'localMark' returned`. Using the returned reference would be undefined behaviour; it is not made safe by the fact that the old bytes sometimes appear to remain in memory.

## Vector growth condition

When `std::vector` grows beyond its capacity, it may allocate a new block and move its elements. References, pointers, and iterators to the old elements then become invalid. Code must not retain a reference across a possible reallocation. An index can be used again after growth if the logical element order and index are still valid. A pointer or reference into a vector is not safer than one into an array in this situation.

## Complexity

Finding the highest of n marks takes O(n) time in the reference, pointer, and index versions, with O(1) extra space. The returned reference avoids copying the selected mark. A copy uses O(1) space for one `int` but cannot update the original. The vector example prints its capacity before appending; whether the append reallocates depends on the reported capacity. In this run the capacity was 3 and the append increased the size to 4, so the old storage had to be replaced.

## Evidence note

The warning is preserved in `compiler_warning.txt`; the safe demonstration output is in `RUN_OUTPUT.txt`. Never run the dangling-reference function to try to obtain an output value.
