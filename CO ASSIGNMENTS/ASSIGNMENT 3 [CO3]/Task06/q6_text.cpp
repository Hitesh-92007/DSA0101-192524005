#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

class Text {
    struct UninitializedTag {};
    char* data_;
    static std::size_t copy_constructions_;

    explicit Text(std::size_t length, UninitializedTag) : data_(new char[length + 1]) {}

public:
    Text() : data_(new char[1]{'\0'}) {}

    explicit Text(const char* source) : data_(nullptr) {
        if (source == nullptr) {
            source = "";
        }
        const std::size_t length = std::strlen(source);
        data_ = new char[length + 1];
        std::memcpy(data_, source, length + 1);
    }

    Text(const Text& other) : data_(nullptr) {
        const std::size_t length = std::strlen(other.data_);
        data_ = new char[length + 1];
        std::memcpy(data_, other.data_, length + 1);
        ++copy_constructions_;
    }

    Text& operator=(const Text& other) {
        if (this == &other) {
            return *this;
        }
        const std::size_t length = std::strlen(other.data_);
        char* replacement = new char[length + 1];
        std::memcpy(replacement, other.data_, length + 1);
        delete[] data_;
        data_ = replacement;
        return *this;
    }

    ~Text() { delete[] data_; }

    Text operator+(const Text& other) const {
        const std::size_t left_length = std::strlen(data_);
        const std::size_t right_length = std::strlen(other.data_);
        Text result(left_length + right_length, UninitializedTag{});
        std::memcpy(result.data_, data_, left_length);
        std::memcpy(result.data_ + left_length, other.data_, right_length + 1);
        return result;
    }

    bool operator==(const Text& other) const { return std::strcmp(data_, other.data_) == 0; }
    bool operator!=(const Text& other) const { return !(*this == other); }
    std::size_t size() const { return std::strlen(data_); }
    const char* c_str() const { return data_; }
    static std::size_t copyConstructions() { return copy_constructions_; }
    static void resetCopyConstructions() { copy_constructions_ = 0; }

    friend std::ostream& operator<<(std::ostream& out, const Text& text) {
        return out << text.data_;
    }
};

std::size_t Text::copy_constructions_ = 0;

int main() {
    Text empty;
    Text greeting("Hello, ");
    Text world("world!");
    Text combined = greeting + world;
    std::cout << "concatenation: " << combined << '\n';
    std::cout << "empty message size: " << empty.size() << '\n';

    Text long_message("This message is intentionally long enough to exercise a heap allocation safely.");
    Text independent = long_message;
    const Text& same_text = independent;
    independent = same_text;
    independent = Text("updated");
    std::cout << "copy independence and self-assignment: " << long_message << " | " << independent << '\n';
    std::cout << "comparison: equal=" << std::boolalpha << (greeting == Text("Hello, "))
              << ", different=" << (greeting != world) << '\n';

    Text::resetCopyConstructions();
    Text expression = greeting + world + Text("!");
    std::cout << "a+b+c: " << expression << ", explicit copy constructions="
              << Text::copyConstructions() << " (C++17 guaranteed elision applies to returned prvalues)\n";

    constexpr int iterations = 100000;
    const Text left("C++ ");
    const Text right("operator+ benchmark");
    volatile std::uint64_t text_checksum = 0;
    const auto text_start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        Text result = left + right;
        text_checksum = text_checksum + static_cast<unsigned char>(result.c_str()[static_cast<std::size_t>(i) % result.size()]);
    }
    const auto text_us = std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::steady_clock::now() - text_start)
                             .count();

    const std::string standard_left("C++ ");
    const std::string standard_right("operator+ benchmark");
    volatile std::uint64_t string_checksum = 0;
    const auto string_start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        const std::string result = standard_left + standard_right;
        string_checksum = string_checksum + static_cast<unsigned char>(result[static_cast<std::size_t>(i) % result.size()]);
    }
    const auto string_us = std::chrono::duration_cast<std::chrono::microseconds>(
                               std::chrono::steady_clock::now() - string_start)
                               .count();
    std::cout << "100,000 concatenations: Text_us=" << text_us << ", std_string_us=" << string_us
              << ", checksums=" << text_checksum << '/' << string_checksum << '\n';
}
