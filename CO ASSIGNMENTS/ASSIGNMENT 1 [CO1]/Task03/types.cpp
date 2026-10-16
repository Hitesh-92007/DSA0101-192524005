#include <climits>
#include <iomanip>
#include <iostream>
int main() {
    std::cout << "bool " << sizeof(bool) << " char " << sizeof(char)
              << " int " << sizeof(int) << " long long " << sizeof(long long)
              << " float " << sizeof(float) << " double " << sizeof(double) << " bytes\n";
    std::cout << "INT_MIN " << INT_MIN << " INT_MAX " << INT_MAX << '\n';
    std::cout << "Maximum network paise " << 3000000LL * 3600LL << '\n';
    double sum = 0.0;
    for (int i = 0; i < 10; ++i) sum += 0.1;
    std::cout << std::setprecision(17) << "Ten additions of 0.1 = " << sum << '\n';
}
