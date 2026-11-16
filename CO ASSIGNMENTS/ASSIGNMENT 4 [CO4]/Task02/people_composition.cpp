#include <iostream>
#include <string>

struct Person {
    std::string name;
    int id;
    std::string phone;
};

struct Student {
    Person person;
    std::string programme;
    double cgpa;
};

struct Staff {
    Person person;
    std::string department;
    double salary;
};

struct TeachingAssistant {
    Student student;
    Staff staff;
};

int main() {
    TeachingAssistant ta{{{"Hitesh", 192524005, "9000000000"}, "CSE", 8.4},
                         {{"Hitesh", 192524005, "9000000000"}, "CSE", 30000}};
    std::cout << "Student name: " << ta.student.person.name << '\n'
              << "Staff department: " << ta.staff.department << '\n'
              << "Two member objects each hold their own Person value.\n"
              << "sizeof(Student)=" << sizeof(Student)
              << " sizeof(Staff)=" << sizeof(Staff)
              << " sizeof(TeachingAssistant)=" << sizeof(TeachingAssistant) << '\n';
}
