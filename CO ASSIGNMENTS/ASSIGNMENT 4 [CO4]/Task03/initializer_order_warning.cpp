#include <iostream>

class Ordered {
    int first_;
    int second_;

public:
    Ordered() : second_(2), first_(1) {}
    void print() const { std::cout << first_ << ' ' << second_ << '\n'; }
};

int main() {
    Ordered value;
    value.print();
}
