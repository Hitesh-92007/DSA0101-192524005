#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class Event {
    std::string name_;
    std::size_t rows_;
    std::size_t seats_per_row_;
    char** seats_;
    std::size_t booked_;

public:
    Event(std::string name, std::size_t rows, std::size_t seats_per_row)
        : name_(std::move(name)), rows_(rows), seats_per_row_(seats_per_row),
          seats_(rows == 0 ? nullptr : new char*[rows]{}), booked_(0) {
        try {
            for (std::size_t row = 0; row < rows_; ++row) {
                seats_[row] = seats_per_row_ == 0 ? nullptr : new char[seats_per_row_];
                if (seats_per_row_ > 0) {
                    std::fill_n(seats_[row], seats_per_row_, '.');
                }
            }
        } catch (...) {
            for (std::size_t row = 0; row < rows_; ++row) {
                delete[] seats_[row];
            }
            delete[] seats_;
            seats_ = nullptr;
            throw;
        }
        std::cout << "construct " << name_ << " (" << rows_ << 'x' << seats_per_row_ << ")\n";
    }

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    ~Event() {
        std::cout << "destroy " << name_ << " (booked=" << booked_ << ")\n";
        for (std::size_t row = 0; row < rows_; ++row) {
            delete[] seats_[row];
        }
        delete[] seats_;
    }

    bool book(std::size_t row, std::size_t seat) {
        if (row >= rows_ || seat >= seats_per_row_) {
            std::cout << "booking rejected: index out of range\n";
            return false;
        }
        if (seats_[row][seat] == 'X') {
            std::cout << "booking rejected: seat already occupied\n";
            return false;
        }
        seats_[row][seat] = 'X';
        ++booked_;
        return true;
    }

    bool cancel(std::size_t row, std::size_t seat) {
        if (row >= rows_ || seat >= seats_per_row_ || seats_[row][seat] != 'X') {
            std::cout << "cancellation rejected\n";
            return false;
        }
        seats_[row][seat] = '.';
        --booked_;
        return true;
    }

    void print() const {
        std::cout << name_ << " seat map:\n";
        for (std::size_t row = 0; row < rows_; ++row) {
            std::cout << "  row " << row << ": ";
            for (std::size_t seat = 0; seat < seats_per_row_; ++seat) {
                std::cout << seats_[row][seat] << (seat + 1 == seats_per_row_ ? '\n' : ' ');
            }
            if (seats_per_row_ == 0) {
                std::cout << "(no seats)\n";
            }
        }
    }

    static std::size_t dynamicBytes(std::size_t rows, std::size_t seats_per_row) {
        return rows * sizeof(char*) + rows * seats_per_row * sizeof(char);
    }
};

int main() {
    Event stack_event("Stack", 2, 3);
    stack_event.book(0, 1);
    stack_event.book(1, 2);
    stack_event.cancel(0, 1);
    stack_event.print();
    stack_event.book(9, 0);

    {
        Event block_event("Block", 1, 2);
        block_event.book(0, 0);
        std::cout << "leaving inner block\n";
    }

#ifdef MISSING_DELETE
    Event* heap_event = new Event("Heap-leak", 10, 20);
    heap_event->book(0, 0);
    std::cout << "leaving main without deleting Heap-leak\n";
#else
    Event* heap_event = new Event("Heap", 1, 2);
    heap_event->book(0, 1);
    delete heap_event;
#endif

    Event zero_rows("Zero-rows", 0, 20);
    zero_rows.print();

    for (const auto dimensions : {std::pair<std::size_t, std::size_t>{10, 20}, {100, 200}, {1000, 2000}}) {
        std::cout << "estimated dynamic seat-map bytes " << dimensions.first << 'x' << dimensions.second
                  << " = " << Event::dynamicBytes(dimensions.first, dimensions.second) << '\n';
    }
}
