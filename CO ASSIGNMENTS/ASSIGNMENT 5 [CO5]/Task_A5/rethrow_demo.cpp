#include <iostream>
#include <string>

struct Guard {
    std::string name;
    explicit Guard(const std::string& value) : name(value) {}
    ~Guard() noexcept { std::cout << '~' << name << ' '; }
};
void f() {
    Guard g("f");
    throw 42;
}
int main() {
    try {
        Guard m("main");
        f();
        std::cout << "not printed ";
    } catch (int x) {
        std::cout << "caught " << x;
    } catch (...) {
        std::cout << "other";
    }
    std::cout << '\n';
}
