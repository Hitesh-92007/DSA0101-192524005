#include <iostream>
#include <string>
#include <utility>

class Address {
public:
    Address(std::string city, int pin) : city_(std::move(city)), pin_(pin) {
        std::cout << "Address constructed\n";
    }
    ~Address() { std::cout << "Address destroyed\n"; }
    std::string str() const { return city_ + "-" + std::to_string(pin_); }

private:
    std::string city_;
    int pin_;
};

class Course {
public:
    explicit Course(std::string title) : title_(std::move(title)) {
        std::cout << "Course constructed\n";
    }
    ~Course() { std::cout << "Course destroyed\n"; }

private:
    std::string title_;
};

class Person {
public:
    Person(std::string name, int age, std::string city, int pin)
        : name_(std::move(name)), age_(age), home_(std::move(city), pin) {
        std::cout << "Person constructed\n";
    }
    virtual ~Person() { std::cout << "Person destroyed\n"; }

protected:
    std::string name_;
    int age_;
    Address home_;
};

class Student : virtual public Person {
public:
    Student(std::string name, int age, std::string city, int pin,
            long long regNo, std::string course)
        : Person(std::move(name), age, std::move(city), pin), regNo_(regNo),
          course_(std::move(course)) {
        std::cout << "Student constructed (ID " << regNo_ << ")\n";
    }
    ~Student() override { std::cout << "Student destroyed\n"; }

private:
    long long regNo_;
    Course course_;
};

class Staff : virtual public Person {
public:
    Staff(std::string name, int age, std::string city, int pin,
          std::string department)
        : Person(std::move(name), age, std::move(city), pin),
          department_(std::move(department)) {
        std::cout << "Staff constructed\n";
    }
    ~Staff() override { std::cout << "Staff destroyed\n"; }

private:
    std::string department_;
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant()
        : Person("Hitesh", 20, "Salem", 636001),
          Student("Hitesh", 20, "ignored", 0, 192524005, "OOP"),
          Staff("Hitesh", 20, "ignored", 0, "CSE") {
        std::cout << "TeachingAssistant constructed\n";
    }
    ~TeachingAssistant() override {
        std::cout << "TeachingAssistant destroyed\n";
    }
};

int main() {
    std::cout << "Student scope\n";
    { Student student("Kavya", 19, "Salem", 636001, 192311001, "OOP"); }
    std::cout << "\nStaff scope\n";
    { Staff staff("Arun", 40, "Chennai", 600001, "CSE"); }
    std::cout << "\nTeachingAssistant scope\n";
    { TeachingAssistant assistant; }
}
