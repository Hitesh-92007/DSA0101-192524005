#include <iostream>

struct Bill {
    virtual ~Bill() { std::cout << "~Bill "; }
    virtual double total() const = 0;
    void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
    ~OPBill() override { std::cout << "~OPBill "; }
    double total() const override { return 500; }
};
int main() {
    Bill* bill = new OPBill;
    bill->show();
    delete bill;
    std::cout << '\n';
}
