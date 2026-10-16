#include <array>
#include <cstddef>
#include <iostream>
enum class Zone { Inner, Middle, Outer, Airport };
constexpr std::array<long long, 4> BASE{{1000, 2000, 3000, 6000}};
constexpr long long MIN_BALANCE = 5000;
int zoneIndex(Zone z) {
    const int i = static_cast<int>(z);
    return i >= 0 && i < static_cast<int>(BASE.size()) ? i : -1;
}
long long fare(Zone z, bool peak, bool weekend, bool passCoversZone) {
    const int i = zoneIndex(z);
    if (i < 0) return -1;
    if (passCoversZone) return 0;
    const long long base = BASE[static_cast<std::size_t>(i)];
    if (weekend) return base * 90 / 100; // no peak surcharge on weekends
    if (peak) return base * 120 / 100;
    return base;
}
class MonthlyPass {
public:
    MonthlyPass(Zone a, Zone b) {
        const int i = zoneIndex(a), j = zoneIndex(b);
        if (i >= 0) zones_[static_cast<std::size_t>(i)] = true;
        if (j >= 0) zones_[static_cast<std::size_t>(j)] = true;
    }
    bool covers(Zone z) const {
        const int i = zoneIndex(z);
        return i >= 0 && zones_[static_cast<std::size_t>(i)];
    }
private:
    std::array<bool, 4> zones_ = {{false, false, false, false}};
};
class SmartCard {
public:
    explicit SmartCard(long long balance) : balance_(balance) {}
    bool pay(long long f) {
        if (f < 0 || balance_ - f < MIN_BALANCE) return false;
        balance_ -= f;
        return true;
    }
    long long balance() const { return balance_; }
private:
    long long balance_;
};
int main() {
    const MonthlyPass pass(Zone::Middle, Zone::Airport);
    SmartCard card(20000);
    const long long weekdayPeak = fare(Zone::Airport, true, false, false);
    const long long weekendPeak = fare(Zone::Airport, true, true, false);
    const long long passFare = fare(Zone::Airport, true, true, pass.covers(Zone::Airport));
    const bool passAccepted = card.pay(passFare);
    std::cout << "Airport weekday peak paise = " << weekdayPeak << '\n'
              << "Airport weekend during peak paise = " << weekendPeak << '\n'
              << "Covered Airport pass fare paise = " << passFare << '\n'
              << "Pass trip accepted = " << (passAccepted ? "yes" : "no")
              << "; balance paise = " << card.balance() << '\n';
}
