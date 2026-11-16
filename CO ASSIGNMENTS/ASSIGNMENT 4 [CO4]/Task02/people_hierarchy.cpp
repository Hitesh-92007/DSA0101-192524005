#include <iostream>
#include <string>

struct Person {
    Person() { std::cout << "Person "; }
    std::string name;
    int id{};
    std::string phone;
};

struct Student : virtual Person {
    Student() { std::cout << "Student "; }
    std::string programme;
    double cgpa{};
};

struct Staff : virtual Person {
    Staff() { std::cout << "Staff "; }
    std::string department;
    double salary{};
};

struct TeachingAssistant : Student, Staff {
    TeachingAssistant() { std::cout << "TA "; }
};

struct ResearchScholar : Student {
    std::string fellowship;
};

int main() {
    TeachingAssistant ta;
    (void)ta;
    std::cout << '\n';
}
