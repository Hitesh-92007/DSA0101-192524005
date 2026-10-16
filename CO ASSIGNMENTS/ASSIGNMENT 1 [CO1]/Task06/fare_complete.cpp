#include <iomanip>
#include <iostream>
enum class Zone { Inner, Middle, Outer };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
long long fare(Zone zone, bool peak) {
    long long base = 0;
    switch (zone) {
        case Zone::Inner: base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer: base = BASE_PAISE[2]; break;
        default: return -1;
    }
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}
bool canTravel(long long balance, long long amount) {
    return amount >= 0 && balance - amount >= MIN_BALANCE_PAISE;
}
void printFare(long long p) {
    std::cout << p / 100 << '.' << std::setw(2) << std::setfill('0')
              << p % 100 << std::setfill(' ');
}
int main() {
    long long balance = 10000;
    struct Trip { Zone zone; bool peak; };
    const Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true},
                          {Zone::Middle, false}};
    for (const Trip& t : trips) {
        const long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs ";
            printFare(f);
        } else std::cout << "Denied (low balance)";
        std::cout << "  balance Rs " << balance / 100 << '\n';
    }
}
