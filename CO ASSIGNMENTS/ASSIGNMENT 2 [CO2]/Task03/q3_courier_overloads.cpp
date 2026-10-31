#include <iomanip>
#include <iostream>

struct Parcel
{
    int weightGrams;
    bool express;
    bool insured;
};

double charge(int weightGrams, bool express = false, bool insured = false)
{
    double total = 40.0 + 0.05 * weightGrams;
    if (express)
    {
        total += 25.0;
    }
    if (insured)
    {
        total += 10.0;
    }
    return total;
}

double charge(double weightKg, bool express = false, bool insured = false)
{
    double total = 40.0 + 35.0 * weightKg;
    if (express)
    {
        total += 25.0;
    }
    if (insured)
    {
        total += 10.0;
    }
    return total;
}

double charge(const Parcel& parcel)
{
    return charge(static_cast<double>(parcel.weightGrams) / 1000.0,
                  parcel.express, parcel.insured);
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "charge(2) -> int grams: Rs " << charge(2) << '\n';
    std::cout << "charge(2.0f) -> double kg: Rs " << charge(2.0f) << '\n';
    std::cout << "charge(2.0) -> double kg: Rs " << charge(2.0) << '\n';
    std::cout << "charge(750, true) -> int grams, express: Rs "
              << charge(750, true) << '\n';
    const Parcel parcel{1250, true, true};
    std::cout << "charge(Parcel) -> Parcel overload: Rs " << charge(parcel) << '\n';
    std::cout << "charge(2.5, false, true) -> double kg, insured: Rs "
              << charge(2.5, false, true) << '\n';
    return 0;
}
