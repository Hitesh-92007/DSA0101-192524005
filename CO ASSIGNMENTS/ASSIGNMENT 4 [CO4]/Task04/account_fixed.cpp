#include <iostream>
#include <memory>
#include <vector>

class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual ~Account() { std::cout << "Account destroyed\n"; }
};

class SeniorAccount final : public Account {
public:
    SeniorAccount() : history_(100, 0.0) {}
    double rate() const override { return 4.0; }
    ~SeniorAccount() override { std::cout << "SeniorAccount destroyed; history released\n"; }

private:
    std::vector<double> history_;
};

int main() {
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());
    for (const auto& account : accounts) std::cout << account->rate() << '\n';
    // Keep derived objects behind pointers/references; do not copy them into Account values.
}
