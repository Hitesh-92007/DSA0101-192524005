#include <iostream>

class PageCount {
public:
#ifdef EXPLICIT_CONSTRUCTOR
    explicit PageCount(int pages) : pages_(pages) {}
#else
    PageCount(int pages) : pages_(pages) {}
#endif

    int value() const { return pages_; }

private:
    int pages_;
};

int main() {
    PageCount pages = 300;
    std::cout << "Implicit conversion produced " << pages.value() << " pages\n";
}
