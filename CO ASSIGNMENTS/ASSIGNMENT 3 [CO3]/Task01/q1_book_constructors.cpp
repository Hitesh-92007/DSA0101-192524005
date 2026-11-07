#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Book {
    std::string title_;
    std::string author_;
    int copies_;

public:
    Book() : Book("", "", 0) {}

    explicit Book(std::string title) : Book(std::move(title), "Unknown", 0) {}

    Book(std::string title, std::string author, int copies)
        : title_(std::move(title)), author_(std::move(author)), copies_(copies) {
        if (copies < 0) {
            throw std::invalid_argument("copy count cannot be negative");
        }
    }

    void print() const {
        std::cout << "title=\"" << title_ << "\", author=\"" << author_
                  << "\", copies=" << copies_ << '\n';
    }
};

class ReorderProbe {
    int first_;
    int second_;

public:
    ReorderProbe() : second_(2), first_(1) {}

    void print() const { std::cout << "reorder probe=" << first_ << ' ' << second_ << '\n'; }
};

int main() {
    try {
        Book empty;
        Book donated("The Pragmatic Programmer");
        Book catalogued("Clean Code", "Robert C. Martin", 3);

        std::cout << "Default constructor: ";
        empty.print();
        std::cout << "Title-only constructor: ";
        donated.print();
        std::cout << "Full constructor: ";
        catalogued.print();

        std::cout << "Array of default-constructed books: ";
        std::vector<Book> empty_records(2);
        std::cout << empty_records.size() << " records created\n";

        try {
            Book invalid("Invalid", "Test", -1);
            invalid.print();
        } catch (const std::invalid_argument& error) {
            std::cout << "Negative-copy test: rejected (" << error.what() << ")\n";
        }

        ReorderProbe probe;
        probe.print();
    } catch (const std::exception& error) {
        std::cerr << "Unexpected error: " << error.what() << '\n';
        return 1;
    }
}
