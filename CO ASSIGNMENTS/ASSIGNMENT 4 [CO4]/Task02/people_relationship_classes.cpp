#include <iostream>
#include <string>
#include <utility>

class Person {
public:
    Person(std::string name, int id, std::string phone)
        : name_(std::move(name)), id_(id), phone_(std::move(phone)) {}
    const std::string& name() const { return name_; }
    int id() const { return id_; }
    const std::string& phone() const { return phone_; }

private:
    std::string name_;
    int id_;
    std::string phone_;
};

class Student : virtual public Person {
public:
    Student(std::string name, int id, std::string phone,
            std::string programme, double cgpa)
        : Person(name, id, phone), programme_(std::move(programme)), cgpa_(cgpa) {}
    const std::string& programme() const { return programme_; }
    double cgpa() const { return cgpa_; }

private:
    std::string programme_;
    double cgpa_;
};

class Staff : virtual public Person {
public:
    Staff(std::string name, int id, std::string phone,
          std::string department, double salary)
        : Person(name, id, phone), department_(std::move(department)), salary_(salary) {}
    const std::string& department() const { return department_; }
    double salary() const { return salary_; }

private:
    std::string department_;
    double salary_;
};

class TeachingAssistant final : public Student, public Staff {
public:
    TeachingAssistant(std::string name, int id, std::string phone,
                      std::string programme, double cgpa,
                      std::string department, double salary)
        : Person(name, id, phone), Student(name, id, phone, programme, cgpa),
          Staff(std::move(name), id, std::move(phone), std::move(department), salary) {}
};

class ResearchScholar final : public Student {
public:
    ResearchScholar(std::string name, int id, std::string phone,
                    std::string programme, double cgpa, std::string fellowship)
        : Person(name, id, phone), Student(std::move(name), id, std::move(phone),
                                          std::move(programme), cgpa),
          fellowship_(std::move(fellowship)) {}
    const std::string& fellowship() const { return fellowship_; }

private:
    std::string fellowship_;
};

int main() {
    TeachingAssistant assistant("Hitesh", 192524005, "9000000000", "CSE", 8.4, "CSE", 30000);
    ResearchScholar scholar("Kavya", 192311001, "9000000001", "CSE", 9.1, "UGC");
    std::cout << "TA: " << assistant.name() << " / ID " << assistant.id()
              << " / " << assistant.phone() << " / " << assistant.programme()
              << " / CGPA " << assistant.cgpa() << " / " << assistant.department()
              << " / salary " << assistant.salary() << '\n'
              << "Scholar: " << scholar.name() << " / ID " << scholar.id()
              << " / " << scholar.programme() << " / CGPA " << scholar.cgpa()
              << " / fellowship " << scholar.fellowship() << '\n';
}
