#include <array>
#include <iomanip>
#include <iostream>
enum class Zone { Inner, Middle, Outer };
constexpr long long BASE[] = {1000, 2000, 3000};
constexpr long long MIN_BALANCE = 5000;
bool peak(int h) { return (h >= 8 && h < 10) || (h >= 17 && h < 20); }
long long fare(Zone z, int h) {
    const int i = static_cast<int>(z);
    if (h < 0 || h > 23 || i < 0 || i >= 3) return -1;
    return peak(h) ? BASE[i] * 120 / 100 : BASE[i];
}
bool canTravel(long long balance, long long f) {
    return f >= 0 && balance - f >= MIN_BALANCE;
}
bool recharge(long long& balance, long long amount) {
    if (amount < 0) return false;
    balance += amount;
    return true;
}
int main() {
    struct Test { const char* name; Zone zone; int hour; long long expected; };
    const std::array<Test, 9> tests{{
        {"Inner off peak", Zone::Inner, 12, 1000},
        {"Middle off peak", Zone::Middle, 12, 2000},
        {"Outer off peak", Zone::Outer, 12, 3000},
        {"08:00 peak", Zone::Inner, 8, 1200},
        {"10:00 off peak", Zone::Inner, 10, 1000},
        {"17:00 peak", Zone::Outer, 17, 3600},
        {"Middle peak", Zone::Middle, 17, 2400},
        {"20:00 off peak", Zone::Outer, 20, 3000},
        {"Invalid hour", Zone::Inner, 24, -1}
    }};
    int failures = 0;
    for (const Test& t : tests) {
        const long long actual = fare(t.zone, t.hour);
        const bool ok = actual == t.expected;
        failures += ok ? 0 : 1;
        std::cout << (ok ? "PASS " : "FAIL ") << t.name
                  << " expected=" << t.expected << " actual=" << actual << '\n';
    }
    std::cout << "Invalid zone rejected: "
              << (fare(static_cast<Zone>(99), 12) < 0 ? "yes" : "no") << '\n';
    std::cout << "Leaves exactly 5000 paise: "
              << (canTravel(6200, 1200) ? "allowed" : "denied") << '\n';
    long long balance = 10000;
    std::cout << "Negative recharge rejected: "
              << (!recharge(balance, -100) ? "yes" : "no") << '\n';
    const std::array<long long, 5> trips{{1000, 2000, 3600, 3000, 1200}};
    long long total = 0;
    for (long long f : trips) total += f;
    std::cout << "Five-trip total paise = " << total
              << " (Rs " << total / 100 << '.' << std::setw(2)
              << std::setfill('0') << total % 100 << std::setfill(' ') << ")\n";
    std::cout << "Failures = " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
