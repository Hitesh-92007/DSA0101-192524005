#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Staff {
public:
    Staff(std::string name, int id) : name_(std::move(name)), id_(id) {}
    virtual ~Staff() = default;

    virtual double pay() const = 0;
    virtual std::string role() const = 0;
    const std::string& name() const { return name_; }
    int id() const { return id_; }

private:
    std::string name_;
    int id_;
};

class Professor final : public Staff {
public:
    Professor(std::string name, int id, double basic, double research)
        : Staff(std::move(name), id), basic_(basic), research_(research) {}
    double pay() const override { return basic_ + basic_ * daRate + research_; }
    std::string role() const override { return "Professor"; }
    static constexpr double daRate = 0.42;

private:
    double basic_;
    double research_;
};

class LabAssistant final : public Staff {
public:
    LabAssistant(std::string name, int id, double basic)
        : Staff(std::move(name), id), basic_(basic) {}
    double pay() const override { return basic_ + basic_ * Professor::daRate; }
    std::string role() const override { return "Lab assistant"; }

private:
    double basic_;
};

class GuestFaculty final : public Staff {
public:
    GuestFaculty(std::string name, int id, double hours, double rate)
        : Staff(std::move(name), id), hours_(hours), rate_(rate) {}
    double pay() const override { return hours_ * rate_; }
    std::string role() const override { return "Guest faculty"; }

private:
    double hours_;
    double rate_;
};

int main() {
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Anita", 101, 50000, 10000));
    payroll.push_back(std::make_unique<LabAssistant>("Ravi", 102, 30000));
    payroll.push_back(std::make_unique<GuestFaculty>("Meera", 103, 20, 1500));

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& member : payroll) {
        std::cout << member->role() << " | " << member->name() << " | ID "
                  << member->id() << " | pay " << member->pay() << '\n';
    }
}
