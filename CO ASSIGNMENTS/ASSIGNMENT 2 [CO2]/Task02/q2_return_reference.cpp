#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

int& highestMark(int marks[], std::size_t count)
{
    if (count == 0)
    {
        throw std::invalid_argument("the marks array must not be empty");
    }

    std::size_t best = 0;
    for (std::size_t i = 1; i < count; ++i)
    {
        if (marks[i] > marks[best])
        {
            best = i;
        }
    }
    return marks[best];
}

int* highestMarkPointer(int marks[], std::size_t count)
{
    if (count == 0)
    {
        return nullptr;
    }

    std::size_t best = 0;
    for (std::size_t i = 1; i < count; ++i)
    {
        if (marks[i] > marks[best])
        {
            best = i;
        }
    }
    return &marks[best];
}

int highestMarkIndex(const int marks[], std::size_t count)
{
    if (count == 0)
    {
        throw std::invalid_argument("the marks array must not be empty");
    }

    std::size_t best = 0;
    for (std::size_t i = 1; i < count; ++i)
    {
        if (marks[i] > marks[best])
        {
            best = i;
        }
    }
    return static_cast<int>(best);
}

int& danglingReference()
{
    int localMark = 75;
    return localMark; // Deliberately unsafe: never call this function.
}

int main()
{
    int marks[] = {78, 91, 84, 88};
    const std::size_t count = sizeof(marks) / sizeof(marks[0]);

    int& top = highestMark(marks, count);
    std::cout << "Highest before moderation: " << top << '\n';
    top += 2;
    std::cout << "Array after +2 through reference: " << marks[1] << '\n';

    const int& readOnlyTop = highestMark(marks, count);
    int copiedTop = readOnlyTop;
    copiedTop = 100;
    std::cout << "Independent copy changed to: " << copiedTop << '\n';
    std::cout << "Const reference sees: " << readOnlyTop << '\n';
    std::cout << "Changing copy leaves array at: " << marks[1] << '\n';

    const int index = highestMarkIndex(marks, count);
    int* pointer = highestMarkPointer(marks, count);
    std::cout << "Index alternative: " << index << " -> " << marks[index] << '\n';
    if (pointer != nullptr)
    {
        std::cout << "Pointer alternative: " << *pointer << '\n';
    }

    std::vector<int> growingRegister = {70, 81, 76};
    const std::size_t oldCapacity = growingRegister.capacity();
    growingRegister.push_back(99);
    std::cout << "Vector size after append: " << growingRegister.size() << '\n';
    std::cout << "A reference into a vector must not be used after a reallocation.\n";
    std::cout << "Old capacity before append: " << oldCapacity << '\n';
    return 0;
}
