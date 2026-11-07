#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

class Money {
    long long paise_;

    struct PaiseTag {};
    Money(long long paise, PaiseTag) : paise_(paise) {}

public:
    Money(long long rupees = 0, int paise = 0) : paise_(rupees * 100 + paise) {}

    Money operator+(const Money& other) const { return Money(paise_ + other.paise_, PaiseTag{}); }
    Money operator-(const Money& other) const { return Money(paise_ - other.paise_, PaiseTag{}); }
    Money& operator+=(const Money& other) {
        paise_ += other.paise_;
        return *this;
    }
    bool operator==(const Money& other) const { return paise_ == other.paise_; }
    bool operator<(const Money& other) const { return paise_ < other.paise_; }
    long long totalPaise() const { return paise_; }

    friend std::ostream& operator<<(std::ostream& out, const Money& amount) {
        const bool negative = amount.paise_ < 0;
        const unsigned long long magnitude = negative
                                                 ? static_cast<unsigned long long>(-(amount.paise_ + 1)) + 1ULL
                                                 : static_cast<unsigned long long>(amount.paise_);
        const auto old_fill = out.fill();
        out << "Rs " << (negative ? "-" : "") << (magnitude / 100ULL) << '.'
            << std::setw(2) << std::setfill('0') << (magnitude % 100ULL);
        out.fill(old_fill);
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Money& amount) {
        std::string token;
        if (!(in >> token)) {
            return in;
        }
        bool negative = false;
        std::size_t start = 0;
        if (!token.empty() && (token[0] == '-' || token[0] == '+')) {
            negative = token[0] == '-';
            start = 1;
        }
        const std::size_t dot = token.find('.', start);
        const std::string whole_text = token.substr(start, dot == std::string::npos ? dot : dot - start);
        const std::string fraction_text = dot == std::string::npos ? "" : token.substr(dot + 1);
        if (whole_text.empty() || fraction_text.size() > 2) {
            in.setstate(std::ios::failbit);
            return in;
        }
        for (const char ch : whole_text + fraction_text) {
            if (ch < '0' || ch > '9') {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        try {
            const long long rupees = std::stoll(whole_text);
            int paise = fraction_text.empty() ? 0 : std::stoi(fraction_text);
            if (fraction_text.size() == 1) {
                paise *= 10;
            }
            const long long total = rupees * 100 + paise;
            amount = Money(negative ? -total : total, PaiseTag{});
        } catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money subtotal = a + b;
    std::cout << "subtotal: " << subtotal << '\n';
    std::cout << std::boolalpha << "subtotal equals Rs 200: " << (subtotal == Money(200)) << '\n';
    Money running_total = a;
    running_total += b;
    std::cout << "+= result: " << running_total << ", Rs 79.25 < Rs 120.75: " << (b < a) << '\n';

    const Money carry = Money(0, 75) + Money(0, 50);
    std::cout << "paise carry (Rs 0.75 + Rs 0.50): " << carry << '\n';
    const Money negative = Money(0, 50) - Money(1, 25);
    std::cout << "negative subtraction: " << negative << '\n';

    Money input;
    std::istringstream valid_amount("12.34");
    valid_amount >> input;
    std::cout << "input parse: " << input << '\n';

    constexpr int iterations = 1000000;
    volatile long long integer_checksum = 0;
    Money current;
    const auto money_start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        current += Money(0, 1);
        integer_checksum = integer_checksum + current.totalPaise();
    }
    const auto money_us = std::chrono::duration_cast<std::chrono::microseconds>(
                              std::chrono::steady_clock::now() - money_start)
                              .count();

    volatile double double_checksum = 0.0;
    double floating = 0.0;
    const auto double_start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        floating += 0.01;
        double_checksum = double_checksum + floating;
    }
    const auto double_us = std::chrono::duration_cast<std::chrono::microseconds>(
                               std::chrono::steady_clock::now() - double_start)
                               .count();
    std::cout << std::setprecision(17);
    std::cout << "1,000,000 additions: Money_us=" << money_us << ", double_us=" << double_us
              << ", final_double=" << floating << ", checksums=" << integer_checksum << '/'
              << double_checksum << '\n';
}
