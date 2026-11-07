# Q3 — Dynamic constructors and destructors

## Design and implementation

`Event` owns a dynamically allocated row-pointer array and a separately allocated character array for each row. Seats start free (`.`), become booked (`X`), and can be cancelled. The constructor is exception-safe: if any row allocation fails, it releases all rows allocated so far and then the row table. Copying is disabled because an unimplemented copy would duplicate the owning pointers. The destructor deletes each row with `delete[]`, then deletes the pointer table with `delete[]`.

The program creates stack, nested-block, and heap events. The heap event is explicitly deleted in the correct build. Compile with `-DMISSING_DELETE` to intentionally omit that deletion for the leak-sanitizer experiment. Zero-row events are accepted and hold no row-table allocation.

## Design answers

- A local stack object's destructor runs when its scope ends. An object in a nested block is destroyed at the closing brace before the enclosing function exits. An object from `new` remains alive until `delete`; omitting it leaks the object and its seat arrays.
- If `delete` is forgotten, the destructor never releases the row table or row arrays. Using scalar `delete` for arrays allocated with `new[]` is undefined behavior; every matching allocation here uses `delete[]`.
- A destructor has no arguments and no return type, so it cannot be overloaded by parameter list. Its single responsibility is to release the resources of the object. Separate cleanup choices should use named member functions or RAII-managed members.
- The dynamic memory layout is one array of `rows` pointers, each pointing to a `seats_per_row` character array. On a 64-bit system the approximate payload for 10×20 is 10×8 + 10×20 = 280 bytes; for 100×200 it is 20,800 bytes; for 1,000×2,000 it is 2,008,000 bytes. These estimates exclude allocator metadata and the `Event` object/string.
- The destructor ties each allocation's release to object lifetime, which is the RAII idea. It makes normal block exit and exception unwinding release memory automatically. A smart-pointer or nested-vector design can provide the same lifetime guarantee with less manual cleanup code.

## Experimental investigation

`RUN_OUTPUT.txt` records construction and destruction order, seat booking/cancellation, rejection of an invalid row, a zero-row event, and the estimated payload sizes. AddressSanitizer runs cleanly for the correct code. The missing-delete variant runs under AddressSanitizer, but this Windows runtime reports that LeakSanitizer is unsupported, so the leak itself is not diagnosed; the missing destructor call is visible in the run record and is the source of the leak. The limitation is recorded in `SANITIZER_RESULTS.txt`.

## Alternative layouts and complexity

With a contiguous `rows × seats` block, storage remains O(rows×seats), access remains O(1) via `row * seats + seat`, and there is one allocation instead of `rows + 1`. It uses no row-pointer table but requires the same seat count in every row. A `std::vector<std::vector<char>>` stores an extra vector object per row and may allocate separately for each row, but manages memory safely and naturally supports different row lengths. All layouts use O(rows×seats) space; row-pointer bookkeeping is O(rows).
