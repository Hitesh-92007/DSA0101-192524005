#include <iostream>

struct Bill {
    ~Bill() { std::cout << "~Bill "; } // deliberately non-virtual for the experiment
    virtual double total() const = 0;
};
struct OPBill : Bill {
    char* resource = new char[128];
    ~OPBill() { delete[] resource; std::cout << "~OPBill "; }
    double total() const override { return 500; }
};
int main() {
    Bill* bill = new OPBill;
    std::cout << bill->total() << ' ';
    delete bill; // undefined behavior; sanitizer may report size mismatch/leak
    std::cout << "finished\n";
}
