#include <string>

class Book {
public:
    explicit Book(std::string title) : title_(title) {}

private:
    std::string title_;
};

int main() {
    Book books[2]; // Expected compile error: no default constructor is available.
    (void)books;
}
