#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>

int readInt(const char* prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return value;
        if (std::cin.eof()) {
            std::cerr << "End of input.\n";
            std::exit(1);
        }
        std::cout << "Invalid number. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int readInRange(const char* prompt, int minimum, int maximum,
                const char* errorMessage) {
    while (true) {
        const int value = readInt(prompt);
        if (value >= minimum && value <= maximum) return value;
        std::cout << errorMessage << '\n';
    }
}

long long farePaise(int zoneNumber, int hour) {
    static const long long baseFare[] = {1000, 2000, 3000};
    long long fare = baseFare[zoneNumber - 1];
    const bool peak = (hour >= 8 && hour < 10) ||
                      (hour >= 17 && hour < 20);
    if (peak) fare += fare * 20 / 100;
    return fare;
}

const char* zoneName(int zoneNumber) {
    switch (zoneNumber) {
        case 1: return "Inner";
        case 2: return "Middle";
        case 3: return "Outer";
        default: return "Invalid";
    }
}

void printMoney(long long paise) {
    std::cout << "Rs " << paise / 100 << '.' << std::setw(2)
              << std::setfill('0') << paise % 100 << std::setfill(' ');
}

int main() {
    const int zoneNumber = readInRange(
        "Zone (1=Inner, 2=Middle, 3=Outer): ", 1, 3, "Invalid zone. Enter 1, 2, or 3.");
    const int hour = readInRange(
        "Departure hour (0-23): ", 0, 23, "Invalid hour. Enter 0 through 23.");
    const int balanceRupees = readInRange(
        "Card balance in rupees: ", 0, std::numeric_limits<int>::max(),
        "Balance cannot be negative.");

    const long long balancePaise = static_cast<long long>(balanceRupees) * 100;
    const long long fare = farePaise(zoneNumber, hour);
    const bool approved = balancePaise >= fare && balancePaise - fare >= 5000;
    const long long balanceAfter = approved ? balancePaise - fare : balancePaise;

    std::cout << "\n===== Journey Report =====\n"
              << "Zone: " << zoneName(zoneNumber) << '\n'
              << "Departure time: " << std::setw(2) << std::setfill('0')
              << hour << ":00" << std::setfill(' ') << '\n'
              << "Fare: ";
    printMoney(fare);
    std::cout << "\nStatus: " << (approved ? "APPROVED" : "DECLINED")
              << "\nBalance after journey: ";
    printMoney(balanceAfter);
    std::cout << '\n';
}
