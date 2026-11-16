#include <iostream>

class Account {
public:
    double rate() const { return 3.5; }
    ~Account() { std::cerr << "Account destructor\n"; }
};

class SeniorAccount : public Account {
    double* history = new double[100];
    static inline int outstandingHistoryBuffers = 0;

public:
    SeniorAccount() { ++outstandingHistoryBuffers; }
    double rate() const { return 4.0; }
    ~SeniorAccount() {
        std::cerr << "SeniorAccount destructor; releasing history\n";
        delete[] history;
        --outstandingHistoryBuffers;
    }
    static int outstandingHistory() { return outstandingHistoryBuffers; }
};

int main() {
    Account* accounts[2] = {new Account, new SeniorAccount};
    for (Account* account : accounts) std::cout << account->rate() << '\n';
    for (Account* account : accounts) delete account;
    std::cout << "Outstanding senior history buffers: "
              << SeniorAccount::outstandingHistory() << '\n';
    Account copy = SeniorAccount();
    std::cout << "Sliced copy base rate: " << copy.rate() << '\n';
}
