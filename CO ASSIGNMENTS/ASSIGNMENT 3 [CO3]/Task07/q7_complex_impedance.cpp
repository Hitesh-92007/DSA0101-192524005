#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>

class Complex {
    double real_;
    double imag_;

public:
    Complex() : real_(0.0), imag_(0.0) {}
    Complex(double real) : real_(real), imag_(0.0) {}
    Complex(double real, double imag) : real_(real), imag_(imag) {}

    double real() const { return real_; }
    double imag() const { return imag_; }
    double magnitude() const { return std::hypot(real_, imag_); }

    Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }
    Complex operator-(const Complex& other) const {
        return Complex(real_ - other.real_, imag_ - other.imag_);
    }
    Complex operator*(const Complex& other) const {
        return Complex(real_ * other.real_ - imag_ * other.imag_,
                       real_ * other.imag_ + imag_ * other.real_);
    }
    Complex operator/(const Complex& other) const {
        const double denominator = other.real_ * other.real_ + other.imag_ * other.imag_;
        if (denominator == 0.0) {
            throw std::domain_error("complex division by zero impedance");
        }
        return Complex((real_ * other.real_ + imag_ * other.imag_) / denominator,
                       (imag_ * other.real_ - real_ * other.imag_) / denominator);
    }
    bool operator==(const Complex& other) const {
        return real_ == other.real_ && imag_ == other.imag_;
    }

    friend std::ostream& operator<<(std::ostream& out, const Complex& value) {
        out << value.real_;
        if (value.imag_ >= 0.0) {
            out << '+';
        }
        out << value.imag_ << 'j';
        return out;
    }
};

Complex parallel(const Complex& first, const Complex& second) {
    return (first * second) / (first + second);
}

int main() {
    const double pi = std::acos(-1.0);
    const double frequency_hz = 50.0;
    const double resistance_ohms = 10.0;
    const double inductance_henry = 0.1;
    const double capacitance_farad = 100e-6;
    const double omega = 2.0 * pi * frequency_hz;
    const double inductive_reactance = omega * inductance_henry;
    const double capacitive_reactance = 1.0 / (omega * capacitance_farad);

    const Complex resistor(resistance_ohms, 0.0);
    const Complex inductor(0.0, inductive_reactance);
    const Complex capacitor(0.0, -capacitive_reactance);
    const Complex series_impedance = resistor + inductor + capacitor;
    const Complex parallel_impedance = parallel(parallel(resistor, inductor), capacitor);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "frequency=" << frequency_hz << " Hz, omega=" << omega << " rad/s\n";
    std::cout << "ZR=" << resistor << " ohm, ZL=" << inductor << " ohm, ZC=" << capacitor << " ohm\n";
    std::cout << "series RLC=" << series_impedance << " ohm, magnitude="
              << series_impedance.magnitude() << " ohm\n";
    std::cout << "parallel RLC=" << parallel_impedance << " ohm, magnitude="
              << parallel_impedance.magnitude() << " ohm\n";
    const Complex arithmetic_check = Complex(5.0, 2.0) - Complex(1.0, 1.0);
    std::cout << "subtraction check=" << arithmetic_check << ", equality check="
              << (arithmetic_check == Complex(4.0, 1.0)) << '\n';

    const std::complex<double> standard_series =
        std::complex<double>(resistance_ohms, 0.0) + std::complex<double>(0.0, inductive_reactance) +
        std::complex<double>(0.0, -capacitive_reactance);
    std::cout << "std::complex check series=" << standard_series.real();
    if (standard_series.imag() >= 0.0) {
        std::cout << '+';
    }
    std::cout << standard_series.imag() << "j\n";

    try {
        const Complex invalid = series_impedance / Complex();
        std::cout << invalid << '\n';
    } catch (const std::domain_error& error) {
        std::cout << "zero-impedance division: rejected (" << error.what() << ")\n";
    }
}
