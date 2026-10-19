#include <climits>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    const auto journeyCount = 3000000LL;
    const float approximateFare = 12.34F;
    const double measuredFare = 12.34;

    std::cout << "sizeof bytes: bool=" << sizeof(bool)
              << " char=" << sizeof(char)
              << " int=" << sizeof(int)
              << " long long=" << sizeof(long long)
              << " unsigned int=" << sizeof(unsigned int)
              << " float=" << sizeof(float)
              << " double=" << sizeof(double)
              << " auto(long long)=" << sizeof(journeyCount) << '\n';

    std::cout << "char range=" << static_cast<int>(CHAR_MIN) << ".."
              << static_cast<int>(CHAR_MAX) << '\n'
              << "int range=" << INT_MIN << ".." << INT_MAX << '\n'
              << "unsigned int range=0.." << UINT_MAX << '\n'
              << "long long range=" << LLONG_MIN << ".." << LLONG_MAX << '\n'
              << "float max=" << std::numeric_limits<float>::max()
              << " (sample Rs " << approximateFare << ")\n"
              << "double max=" << std::numeric_limits<double>::max()
              << " (sample Rs " << measuredFare << ")\n";

    const long long maximumNetworkPaise = journeyCount * 3600LL;
    std::cout << "Maximum network paise at Rs 36 per journey = "
              << maximumNetworkPaise << '\n'
              << "Network total fits int = "
              << (maximumNetworkPaise <= INT_MAX ? "yes" : "no") << '\n';

    double repeatedSum = 0.0;
    for (int i = 0; i < 10; ++i) repeatedSum += 0.1;
    std::cout << std::setprecision(17)
              << "Ten additions of 0.1 = " << repeatedSum << '\n';
}
