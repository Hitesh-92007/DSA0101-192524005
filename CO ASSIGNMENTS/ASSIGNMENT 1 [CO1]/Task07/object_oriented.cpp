#include <iostream>
class SmartCard {
public:
    explicit SmartCard(long long balance) : balance_(balance) {}
    bool canTravel(long long fare) const {
        return fare >= 0 && balance_ - fare >= MIN_BALANCE;
    }
    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }
    long long balance() const { return balance_; }
private:
    static constexpr long long MIN_BALANCE = 5000;
    long long balance_;
};
int main() {
    SmartCard card(10000);
    const bool allowed = card.deduct(3600);
    std::cout << (allowed ? "Allowed" : "Denied")
              << "; remaining paise = " << card.balance() << '\n';
}
