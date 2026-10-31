#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

class Time
{
    int h;
    int m;

public:
    Time(int hour, int minute) : h(hour), m(minute) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

#ifdef USE_INLINE
inline int minutesBetween(const Time& a, const Time& b)
#else
int minutesBetween(const Time& a, const Time& b)
#endif
{
    const int first = a.h * 60 + a.m;
    const int second = b.h * 60 + b.m;
    const int difference = first - second;
    return difference < 0 ? -difference : difference;
}

#ifdef USE_INLINE
inline bool earlier(const Time& a, const Time& b)
#else
bool earlier(const Time& a, const Time& b)
#endif
{
    return a.h * 60 + a.m < b.h * 60 + b.m;
}

int main()
{
    const Time departure(9, 45), arrival(13, 10);
    std::cout << "Given program result: " << minutesBetween(departure, arrival)
              << ' ' << std::boolalpha << earlier(arrival, departure) << '\n';

    const Time beforeNoon(11, 55), afterNoon(12, 10);
    std::cout << "Across noon: " << minutesBetween(beforeNoon, afterNoon)
              << ' ' << earlier(beforeNoon, afterNoon) << '\n';
    const Time sameA(14, 5), sameB(14, 5);
    std::cout << "Same time: " << minutesBetween(sameA, sameB)
              << ' ' << earlier(sameA, sameB) << '\n';

    std::mt19937 generator(192524005U);
    std::uniform_int_distribution<int> hour(0, 23);
    std::uniform_int_distribution<int> minute(0, 59);
    std::vector<std::pair<Time, Time>> cases;
    for (int i = 0; i < 1024; ++i)
    {
        cases.emplace_back(Time(hour(generator), minute(generator)),
                           Time(hour(generator), minute(generator)));
    }

    const std::uint64_t calls = 10000000ULL;
    std::uint64_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::uint64_t i = 0; i < calls; ++i)
    {
        const std::size_t index = static_cast<std::size_t>(i & 1023ULL);
        checksum += static_cast<std::uint64_t>(
            minutesBetween(cases[index].first, cases[index].second));
    }
    const auto finish = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        finish - start).count();

#ifdef USE_INLINE
    std::cout << "Inline case";
#else
    std::cout << "Non-inline case";
#endif
    std::cout << ": calls=" << calls << " checksum=" << checksum
              << " elapsed_us=" << elapsed << '\n';
    return 0;
}
