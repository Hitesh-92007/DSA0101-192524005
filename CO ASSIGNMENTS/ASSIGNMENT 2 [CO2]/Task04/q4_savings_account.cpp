#include <iostream>
#include <string>

class SavingsAccount
{
private:
    struct Transaction
    {
        std::string label;
        long long change;
        long long balanceAfter;
    };

    static const int HISTORY_SIZE = 10;
    long long accountNumber_;
    long long balance_;
    Transaction history_[HISTORY_SIZE];
    int count_;
    int start_;

    void record(const std::string& label, long long change);

public:
    SavingsAccount(long long accountNumber, long long openingBalance)
        : accountNumber_(accountNumber), balance_(openingBalance), count_(0), start_(0)
    {
    }

    bool deposit(long long amount)
    {
        if (amount <= 0)
        {
            return false;
        }
        balance_ += amount;
        record("Deposit", amount);
        return true;
    }

    bool withdraw(long long amount);
    void miniStatement() const;
    long long balance() const { return balance_; }
};

void SavingsAccount::record(const std::string& label, long long change)
{
    int position;
    if (count_ < HISTORY_SIZE)
    {
        position = (start_ + count_) % HISTORY_SIZE;
        ++count_;
    }
    else
    {
        position = start_;
        start_ = (start_ + 1) % HISTORY_SIZE;
    }
    history_[position] = Transaction{label, change, balance_};
}

bool SavingsAccount::withdraw(long long amount)
{
    if (amount <= 0 || amount > balance_)
    {
        return false;
    }
    balance_ -= amount;
    record("Withdrawal", -amount);
    return true;
}

void SavingsAccount::miniStatement() const
{
    std::cout << "Mini-statement for account " << accountNumber_ << '\n';
    for (int i = 0; i < count_; ++i)
    {
        const int position = (start_ + i) % HISTORY_SIZE;
        const Transaction& transaction = history_[position];
        std::cout << i + 1 << ". " << transaction.label << ' '
                  << (transaction.change > 0 ? '+' : '-')
                  << (transaction.change > 0 ? transaction.change : -transaction.change)
                  << " | balance " << transaction.balanceAfter << '\n';
    }
    std::cout << "Current balance: " << balance_ << '\n';
}

int main()
{
    SavingsAccount account(192524005, 1000);
    account.deposit(200);
    account.withdraw(100);
    account.deposit(300);
    account.withdraw(50);
    account.deposit(100);
    account.withdraw(25);
    account.deposit(75);
    account.withdraw(40);
    account.deposit(60);
    account.withdraw(30);
    account.deposit(90);
    account.withdraw(20);
    account.miniStatement();
    return 0;
}
