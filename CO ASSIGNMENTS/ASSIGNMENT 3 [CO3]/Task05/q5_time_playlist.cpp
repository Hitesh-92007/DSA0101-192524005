#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Time {
    static constexpr int seconds_per_day = 24 * 60 * 60;
    int seconds_;

public:
    explicit Time(int hour = 0, int minute = 0, int second = 0)
        : seconds_(hour * 3600 + minute * 60 + second) {
        if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60 || second < 0 || second >= 60) {
            throw std::invalid_argument("time fields are out of range");
        }
    }

    int totalSeconds() const { return seconds_; }

    Time& operator++() {
        seconds_ = (seconds_ + 1) % seconds_per_day;
        return *this;
    }

    Time operator++(int) {
        Time old = *this;
        ++(*this);
        return old;
    }

    bool operator==(const Time& other) const { return seconds_ == other.seconds_; }
    bool operator<(const Time& other) const { return seconds_ < other.seconds_; }

    friend std::ostream& operator<<(std::ostream& out, const Time& time) {
        const int hour = time.seconds_ / 3600;
        const int minute = (time.seconds_ / 60) % 60;
        const int second = time.seconds_ % 60;
        const auto old_fill = out.fill();
        out << std::setw(2) << std::setfill('0') << hour << ':'
            << std::setw(2) << std::setfill('0') << minute << ':'
            << std::setw(2) << std::setfill('0') << second;
        out.fill(old_fill);
        return out;
    }
};

class Playlist {
    std::vector<std::string> tracks_;

public:
    Playlist(std::initializer_list<std::string> tracks) : tracks_(tracks) {}
    std::size_t size() const { return tracks_.size(); }

    std::string& operator[](int index) {
        if (index < 0 || static_cast<std::size_t>(index) >= tracks_.size()) {
            throw std::out_of_range("playlist index out of range");
        }
        return tracks_[static_cast<std::size_t>(index)];
    }

    const std::string& operator[](int index) const {
        if (index < 0 || static_cast<std::size_t>(index) >= tracks_.size()) {
            throw std::out_of_range("playlist index out of range");
        }
        return tracks_[static_cast<std::size_t>(index)];
    }
};

struct Counter {
    int value;
    Counter& operator++() { ++value; return *this; }
    Counter operator++(int) { Counter old = *this; ++value; return old; }
};

template <typename Increment>
long long benchmark(Increment increment, std::uint64_t& checksum) {
    constexpr int iterations = 10000000;
    Time time;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        increment(time, checksum);
    }
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::steady_clock::now() - start)
        .count();
}

int main() {
    Counter c{5};
    Counter a = c++;
    Counter b = ++c;
    std::cout << "counter prediction program: " << a.value << ' ' << b.value << ' ' << c.value << '\n';

    Time end_of_day(23, 59, 59);
    ++end_of_day;
    std::cout << "24-hour rollover: " << end_of_day << '\n';
    Time minute_rollover(0, 0, 59);
    ++minute_rollover;
    std::cout << "minute rollover: " << minute_rollover << '\n';
    Time hour_rollover(0, 59, 59);
    ++hour_rollover;
    std::cout << "hour rollover: " << hour_rollover << '\n';
    std::cout << "time comparison: 00:00:01 < 00:00:02 is "
              << std::boolalpha << (Time(0, 0, 1) < Time(0, 0, 2)) << ", equality is "
              << (Time(0, 0, 1) == Time(0, 0, 1)) << '\n';

    Playlist playlist{"Blue in Green", "So What", "Take Five"};
    const Playlist& read_only_playlist = playlist;
    playlist[1] = "So What (live)";
    std::cout << "playback: ";
    Time position(0, 0, 0);
    for (int i = 0; i < static_cast<int>(playlist.size()); ++i) {
        std::cout << position << ' ' << read_only_playlist[i] << '\n';
        ++position;
    }
    for (const int bad_index : {-1, static_cast<int>(playlist.size())}) {
        try {
            std::cout << playlist[bad_index] << '\n';
        } catch (const std::out_of_range& error) {
            std::cout << "playlist[" << bad_index << "]: " << error.what() << '\n';
        }
    }

    std::uint64_t prefix_checksum = 0;
    const long long prefix_us = benchmark(
        [](Time& time, std::uint64_t& checksum) { ++time; checksum += static_cast<std::uint64_t>(time.totalSeconds()); },
        prefix_checksum);
    std::uint64_t postfix_checksum = 0;
    const long long postfix_us = benchmark(
        [](Time& time, std::uint64_t& checksum) { Time old = time++; checksum += static_cast<std::uint64_t>(old.totalSeconds()); },
        postfix_checksum);
    std::cout << "10,000,000 increments: prefix_us=" << prefix_us << ", postfix_us=" << postfix_us
              << ", checksums=" << prefix_checksum << '/' << postfix_checksum << '\n';
}
