#include <iomanip>
#include <iostream>
#include <limits>
#include <cstdlib>
int readInt(const char* prompt) {
    int value{};
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
void printMoney(long long paise) {
    std::cout << "Rs " << paise / 100 << '.' << std::setw(2)
              << std::setfill('0') << paise % 100 << std::setfill(' ');
}
int main() {
    const int hour = readInt("Hour (0-23): ");
    const int recharge = readInt("Recharge in rupees: ");
    if (hour < 0 || hour > 23) { std::cout << "Invalid hour.\n"; return 1; }
    if (recharge < 0) { std::cout << "Negative recharge rejected.\n"; return 1; }
    const long long paise = static_cast<long long>(recharge) * 100;
    std::cout << "Accepted recharge = ";
    printMoney(paise);
    std::cout << " (" << paise << " paise)\n";
}
