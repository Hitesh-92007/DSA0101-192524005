#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#if defined(__clang__) || defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE
#endif

NOINLINE std::size_t scanHighestIndex(const std::vector<int>& marks)
{
    std::size_t best = 0;
    for (std::size_t i = 1; i < marks.size(); ++i)
    {
        if (marks[i] > marks[best])
        {
            best = i;
        }
    }
    return best;
}

class CachedMarkRegister
{
    std::vector<int> marks_;
    std::size_t highest_;

    void refreshHighest()
    {
        highest_ = scanHighestIndex(marks_);
    }

public:
    explicit CachedMarkRegister(const std::vector<int>& marks)
        : marks_(marks), highest_(0)
    {
        if (marks_.empty())
        {
            throw std::invalid_argument("mark register must not be empty");
        }
        refreshHighest();
    }

    std::size_t highestIndex() const
    {
        return highest_;
    }

    void update(std::size_t index, int newMark)
    {
        if (index >= marks_.size())
        {
            throw std::out_of_range("mark index is outside the register");
        }
        const bool loweredCurrentHighest =
            index == highest_ && newMark < marks_[highest_];
        marks_[index] = newMark;
        if (loweredCurrentHighest)
        {
            refreshHighest();
        }
        else if (newMark > marks_[highest_])
        {
            highest_ = index;
        }
    }
};

int main()
{
    const std::vector<int> sample = {78, 91, 84, 88};
    CachedMarkRegister sampleRegister(sample);
    std::cout << "Sample maximum index before update: "
              << sampleRegister.highestIndex() << '\n';
    sampleRegister.update(1, 80);
    std::cout << "Sample maximum index after lowering old maximum: "
              << sampleRegister.highestIndex() << '\n';

    const std::size_t markCount = 10000;
    const std::size_t queryCount = 1000;
    std::mt19937 generator(92007U);
    std::uniform_int_distribution<int> score(0, 99999);
    std::vector<int> marks;
    marks.reserve(markCount);
    for (std::size_t i = 0; i < markCount; ++i)
    {
        marks.push_back(score(generator));
    }
    marks[markCount / 2] = 100000;
    CachedMarkRegister cached(marks);

    std::uint64_t scanChecksum = 0;
    const auto scanStart = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < queryCount; ++i)
    {
        marks[0] = static_cast<int>(i % 97);
        scanChecksum += scanHighestIndex(marks);
    }
    const auto scanFinish = std::chrono::steady_clock::now();

    std::uint64_t cachedChecksum = 0;
    const auto cachedStart = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < queryCount; ++i)
    {
        cached.update(0, static_cast<int>(i % 97));
        cachedChecksum += cached.highestIndex();
    }
    const auto cachedFinish = std::chrono::steady_clock::now();

    const auto scanUs = std::chrono::duration_cast<std::chrono::microseconds>(
        scanFinish - scanStart).count();
    const auto cachedUs = std::chrono::duration_cast<std::chrono::microseconds>(
        cachedFinish - cachedStart).count();
    const std::size_t scannedIndex = scanHighestIndex(marks);
    const std::size_t cachedIndex = cached.highestIndex();
    std::cout << "marks=" << markCount << " queries=" << queryCount << '\n';
    std::cout << "scan index=" << scannedIndex << " checksum=" << scanChecksum
              << " elapsed_us=" << scanUs << '\n';
    std::cout << "cached index=" << cachedIndex << " checksum=" << cachedChecksum
              << " elapsed_us=" << cachedUs << '\n';
    std::cout << "same result: " << std::boolalpha
              << (scannedIndex == cachedIndex && scanChecksum == cachedChecksum)
              << '\n';
    return 0;
}
