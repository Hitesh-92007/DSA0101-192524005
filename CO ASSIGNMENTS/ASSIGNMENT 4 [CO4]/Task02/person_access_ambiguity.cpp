#include <string>

struct Person { std::string name; };
struct Student : Person {};
struct Staff : Person {};
struct TeachingAssistant : Student, Staff {};

int main() {
    TeachingAssistant ta;
    return static_cast<int>(ta.name.size()); // Expected error: name is ambiguous.
}
