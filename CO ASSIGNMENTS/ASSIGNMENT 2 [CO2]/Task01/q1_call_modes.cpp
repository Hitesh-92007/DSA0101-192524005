#include <iostream>

void change(int a, int& b, int* c)
{
    a += 10;
    b += 10;
    *c += 10;
}

int& bigger(int& p, int& q)
{
    return p > q ? p : q;
}

void show(int x, int y = 5, int z = 10)
{
    std::cout << x + y + z << ' ';
}

void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;
}

void applyConcession(long long& fee, int percent)
{
    fee -= fee * percent / 100;
}

void applyByPointer(long long* fee, int percent)
{
    if (fee != nullptr)
    {
        *fee -= *fee * percent / 100;
    }
}

int main()
{
    std::cout << "Given program output:\n";
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';

    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << '\n';

    std::cout << "Concession comparison on separate 45000 fees:\n";
    long long byValue = 45000;
    long long byReference = 45000;
    long long byPointer = 45000;
    applyByValue(byValue, 10);
    applyConcession(byReference, 15);
    applyByPointer(&byPointer, 25);
    std::cout << "value (10%): " << byValue << '\n';
    std::cout << "reference (15%): " << byReference << '\n';
    std::cout << "pointer (25%): " << byPointer << '\n';

    std::cout << "Completed reference TODO:\n";
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value : " << hostelFee << '\n';
    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';

    long long combinedFee = 45000;
    applyConcession(combinedFee, 10);
    applyConcession(combinedFee, 15);
    applyConcession(combinedFee, 25);
    std::cout << "combined 10%, 15%, 25% (integer rupees): "
              << combinedFee << '\n';
    return 0;
}
