// Deliberate compile-failure demonstration for B1. Removing virtual from a
// pure virtual function declaration is ill-formed in C++.
struct Bill {
    double total() const = 0; // expected diagnostic: only virtual functions can be pure
};
int main() { return 0; }
