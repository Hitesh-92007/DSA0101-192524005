#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

class Prescription {
    int n_;
    std::string* medicines_;

public:
    static int copy_constructions;

    explicit Prescription(int count)
        : n_(count), medicines_(count > 0 ? new std::string[static_cast<std::size_t>(count)] : nullptr) {
        if (count < 0) {
            throw std::invalid_argument("medicine count cannot be negative");
        }
    }

#ifdef SHALLOW_VERSION
    Prescription(const Prescription& other) : n_(other.n_), medicines_(other.medicines_) {
        ++copy_constructions;
    }
#else
    Prescription(const Prescription& other)
        : n_(other.n_), medicines_(other.n_ > 0 ? new std::string[static_cast<std::size_t>(other.n_)] : nullptr) {
        ++copy_constructions;
        if (n_ > 0) {
            try {
                std::copy(other.medicines_, other.medicines_ + n_, medicines_);
            } catch (...) {
                delete[] medicines_;
                throw;
            }
        }
    }

    Prescription& operator=(const Prescription& other) {
        if (this == &other) {
            return *this;
        }
        std::string* replacement = other.n_ > 0
                                       ? new std::string[static_cast<std::size_t>(other.n_)]
                                       : nullptr;
        try {
            if (other.n_ > 0) {
                std::copy(other.medicines_, other.medicines_ + other.n_, replacement);
            }
        } catch (...) {
            delete[] replacement;
            throw;
        }
        delete[] medicines_;
        n_ = other.n_;
        medicines_ = replacement;
        return *this;
    }
#endif

    ~Prescription() { delete[] medicines_; }

    int size() const { return n_; }

    void set(int index, const std::string& medicine) {
        if (index < 0 || index >= n_) {
            throw std::out_of_range("medicine index out of range");
        }
        medicines_[index] = medicine;
    }

    const std::string& get(int index) const {
        if (index < 0 || index >= n_) {
            throw std::out_of_range("medicine index out of range");
        }
        return medicines_[index];
    }
};

int Prescription::copy_constructions = 0;

void inspectByValue(Prescription prescription) {
    std::cout << "by-value count=" << prescription.size() << '\n';
}

void inspectByReference(const Prescription& prescription) {
    std::cout << "by-reference count=" << prescription.size() << '\n';
}

int main() {
#ifdef SHALLOW_VERSION
    Prescription original(2);
    original.set(0, "Paracetamol");
    original.set(1, "ORS");
    Prescription copied = original;
    copied.set(0, "Cetirizine");
    std::cout << "shallow alias=" << original.get(0) << '\n';
    return 0; // Expected AddressSanitizer double-free when both objects are destroyed.
#else
    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");

    Prescription q = p;
    q.set(1, "Cetirizine");

    Prescription r(1);
    r = p;
    p.set(0, "Ibuprofen");

    std::cout << "deep-copy independence: " << p.get(0) << ' ' << p.get(1) << " | "
              << q.get(0) << ' ' << q.get(1) << " | " << r.get(0) << ' ' << r.get(1) << '\n';
    const Prescription& same_object = p;
    p = same_object;
    std::cout << "self-assignment: " << p.get(0) << ' ' << p.get(1) << '\n';

    Prescription::copy_constructions = 0;
    inspectByValue(p);
    const int by_value_copies = Prescription::copy_constructions;
    Prescription::copy_constructions = 0;
    inspectByReference(p);
    const int by_reference_copies = Prescription::copy_constructions;
    std::cout << "parameter copies: value=" << by_value_copies
              << ", const-reference=" << by_reference_copies << '\n';

    for (const int count : {10, 1000, 100000}) {
        Prescription source(count);
        for (int i = 0; i < count; ++i) {
            source.set(i, "Medicine-" + std::to_string(i));
        }
        const int repetitions = std::max(1, 100000 / count);
        volatile std::size_t checksum = 0;
        const auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < repetitions; ++i) {
            Prescription copy(source);
            checksum = checksum + copy.get(count - 1).size();
        }
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                                 std::chrono::steady_clock::now() - start)
                                 .count();
        std::cout << "copy benchmark: medicines=" << count << ", repetitions=" << repetitions
                  << ", total_us=" << elapsed << ", checksum=" << checksum << '\n';
    }
#endif
}
