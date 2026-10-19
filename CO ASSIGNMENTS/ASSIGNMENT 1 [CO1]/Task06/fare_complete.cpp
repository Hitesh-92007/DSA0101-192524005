#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

const char* zoneName(Zone zone) {
    switch (zone) {
        case Zone::Inner: return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer: return "Outer";
        default: return "Invalid";
    }
}

long long fare(Zone zone, int hour) {
    if (hour < 0 || hour > 23) return -1;

    long long amount = 0;
    switch (zone) {
        case Zone::Inner: amount = BASE_PAISE[0]; break;
        case Zone::Middle: amount = BASE_PAISE[1]; break;
        case Zone::Outer: amount = BASE_PAISE[2]; break;
        default: return -1;
    }

    if (isPeakHour(hour))
        amount += amount * PEAK_SURCHARGE_PERCENT / 100;
    return amount;
}

bool canTravel(long long balance, long long amount) {
    return amount >= 0 && balance >= amount &&
           balance - amount >= MIN_BALANCE_PAISE;
}

void printMoney(long long paise) {
    std::cout << paise / 100 << '.' << std::setw(2) << std::setfill('0')
              << paise % 100 << std::setfill(' ');
}

struct Trip {
    Zone zone;
    int hour;
};

int main() {
    long long balance = 10000;
    const Trip sampleTrips[] = {
        {Zone::Inner, 12}, {Zone::Outer, 17}, {Zone::Middle, 12}
    };

    std::cout << "Sample journeys:\n";
    for (const Trip& trip : sampleTrips) {
        const long long amount = fare(trip.zone, trip.hour);
        if (canTravel(balance, amount)) {
            balance -= amount;
            std::cout << "Fare Rs ";
            printMoney(amount);
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << "  balance Rs " << balance / 100 << '\n';
    }

    const Trip boundaryTrips[] = {
        {Zone::Inner, 10}, {Zone::Outer, 20}
    };
    std::cout << "Boundary journey checks:\n";
    for (const Trip& trip : boundaryTrips) {
        long long testBalance = 10000;
        const long long amount = fare(trip.zone, trip.hour);
        const bool allowed = canTravel(testBalance, amount);
        if (allowed) testBalance -= amount;
        std::cout << std::setw(2) << std::setfill('0') << trip.hour
                  << ":00 " << zoneName(trip.zone) << " is "
                  << (isPeakHour(trip.hour) ? "peak" : "off-peak")
                  << "; fare Rs ";
        printMoney(amount);
        std::cout << "; " << (allowed ? "allowed" : "denied")
                  << "; balance Rs " << testBalance / 100 << '\n';
    }
}
