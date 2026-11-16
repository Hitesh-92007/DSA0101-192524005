#include <iomanip>
#include <iostream>
#include <string>

class Cart {
    std::string items_[10];
    double prices_[10]{};
    int count_ = 0;
    double discount_ = 0;

public:
    Cart& add(const std::string& item, double price) {
        if (count_ < 10 && price >= 0) {
            items_[count_] = item;
            prices_[count_++] = price;
        }
        return *this;
    }
    Cart& setDiscount(double discount) {
        if (discount >= 0 && discount <= 100) this->discount_ = discount;
        return *this;
    }
    double total() const {
        double sum = 0;
        for (int i = 0; i < count_; ++i) sum += prices_[i];
        return sum * (100.0 - discount_) / 100.0;
    }
};

int main() {
    Cart named;
    named.add("Rice", 60); // direct object
    Cart* pointer = &named;
    pointer->add("Dal", 120); // pointer and ->
    Cart& reference = named;
    reference.setDiscount(10); // reference
    pointer->add("Oil", 180);

    Cart* dynamic = new Cart;
    dynamic->add("Oil", 180);
    const double dynamicTotal = dynamic->total();
    delete dynamic;

    Cart one; one.add("Book", 250);
    Cart two; two.add("Bag", 900);
    Cart three; three.add("Pen", 40);
    Cart* activeCarts[] = {&one, &two, &three};
    Cart* highest = activeCarts[0];
    for (Cart* cart : activeCarts) {
        if (cart->total() > highest->total()) highest = cart;
    }

    std::cout << std::fixed << std::setprecision(2)
              << "Chained cart total: " << pointer->total() << '\n'
              << "Dynamic cart total before delete: " << dynamicTotal << '\n'
              << "Highest active cart total: " << highest->total() << '\n';
}
