#include <climits>
#include <iomanip>
#include <iostream>
int main() {
    constexpr long long counts[] = {20000, 500000, 3000000};
    constexpr long long farePaise = 1234; // Rs 12.34, precision demonstration
    for (long long n : counts) {
        long long paise = 0;
        double rupees = 0.0;
        for (long long i = 0; i < n; ++i) {
            paise += farePaise;
            rupees += 12.34;
        }
        const double exactRupees = static_cast<double>(paise) / 100.0;
        std::cout << "journeys=" << n << " paise=" << paise
                  << std::setprecision(17) << " double=" << rupees
                  << " difference=" << rupees - exactRupees << '\n';
    }
    const long long networkPaise = 3000000LL * 3600LL;
    std::cout << "INT_MAX=" << INT_MAX << " network paise=" << networkPaise
              << " fits int=" << (networkPaise <= INT_MAX ? "yes" : "no") << '\n';
}
