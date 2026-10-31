#include <iostream>
#include <string>

class Token
{
    static int next;
    int id;

public:
    Token() : id(++next) {}
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100;

class BusTicket
{
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& passengerName)
        : seat(0), name(passengerName)
    {
        if (issued < CAPACITY)
        {
            seat = ++issued;
        }
    }

    static int count()
    {
        return issued;
    }

    void print() const
    {
        std::cout << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }
};

int BusTicket::issued = 0;

void createMoreTickets()
{
    BusTicket laterBusTicket("Esha");
    laterBusTicket.print();
    std::cout << "Shared count inside second call: " << BusTicket::count() << '\n';
}

int main()
{
    std::cout << "Given Token program output:\n";
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' '
              << extra.get() << ' ' << Token::issued() << '\n';

    std::cout << "Bus ticket array output:\n";
    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"),
                         BusTicket("Chitra"), BusTicket("Dev")};
    for (const BusTicket& ticket : trip)
    {
        ticket.print();
    }
    std::cout << "Issued: " << BusTicket::count() << '\n';

    std::cout << "Ticket created after the capacity is reached:\n";
    createMoreTickets();
    createMoreTickets();
    return 0;
}
