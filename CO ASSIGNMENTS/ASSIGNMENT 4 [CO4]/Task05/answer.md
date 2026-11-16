# Task 5 Pointers to Objects and the `this` Pointer

## Four ways to use a Cart

`cart_access_ways.cpp` shows direct access through a named object, access through a pointer using `->`, access through a reference, and an explicitly allocated object released with `delete`. A named object or reference is the simplest choice when the lifetime is local and automatic. A non-owning pointer can refer to an existing object and can be null, so check it before use. Dynamic allocation is appropriate only when a lifetime must outlast the current scope; in modern C++, prefer `std::unique_ptr` so deletion is automatic.

## `this` and chaining

Inside a non-static member function, `this` points to the current object. `*this` is that object as a reference. In `setDiscount(double discount)`, `this->discount_` names the member while `discount` names the parameter. Returning `Cart&` from `add()` and `setDiscount()` returns the same object, enabling chained calls without copying it. Returning `Cart` by value would copy the cart and subsequent chained calls would operate on the copy.

The completed code from the question is in `cart_complete.cpp`; it prints `324.00`.

## Highest-total search

The program loops through a fixed array of `Cart*` values and retains the pointer with the largest `total()`. For n carts, this takes O(n) time and O(1) extra search space. Indexing and pointer arithmetic both visit the same entries.

## Vector alternative

Searching `std::vector<Cart>` also takes O(n) time. It stores complete Cart objects in contiguous storage, while an array of pointers stores one pointer per entry and the objects live elsewhere. A pointer or reference to a vector element can become invalid when growth reallocates the vector. Reserve enough capacity or reacquire references after mutation. A `vector<unique_ptr<Cart>>` keeps the separate Cart objects at stable addresses when the vector of smart pointers grows.
