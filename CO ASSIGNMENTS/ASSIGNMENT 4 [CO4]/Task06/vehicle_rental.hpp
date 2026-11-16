#ifndef CO4_VEHICLE_RENTAL_HPP
#define CO4_VEHICLE_RENTAL_HPP

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

struct ServiceDate {
    int year;
    int month;
    int day;

    std::string str() const {
        std::ostringstream out;
        out << std::setfill('0') << std::setw(4) << year << '-'
            << std::setw(2) << month << '-' << std::setw(2) << day;
        return out.str();
    }
};

class Vehicle {
public:
    Vehicle(std::string registration, double dailyRate, double lateFeePerDay)
        : registration_(std::move(registration)), dailyRate_(dailyRate),
          lateFeePerDay_(lateFeePerDay) {}
    virtual ~Vehicle() = default;

    virtual std::string type() const = 0;
    virtual double rentalCharge(int days) const = 0;
    virtual std::string details() const = 0;

    bool rent(int days) {
        if (rented_ || days < 1) return false;
        bookedDays_ = days;
        rented_ = true;
        return true;
    }

    double returnVehicle(int lateDays) {
        if (!rented_ || lateDays < 0) return -1.0;
        const double total = rentalCharge(bookedDays_ + lateDays) + lateFeePerDay_ * lateDays;
        rented_ = false;
        bookedDays_ = 0;
        return total;
    }

    std::string registration() const { return registration_; }
    bool isRented() const { return rented_; }
    double dailyRate() const { return dailyRate_; }
    std::string listing() const {
        return type() + " | " + registration_ + " | " +
               (rented_ ? "Rented" : "Available") + " | " + details();
    }

protected:
    double baseCharge(int days) const { return dailyRate_ * days; }

private:
    std::string registration_;
    double dailyRate_;
    double lateFeePerDay_;
    int bookedDays_ = 0;
    bool rented_ = false;
};

class Bicycle final : public Vehicle {
public:
    Bicycle(std::string reg, double rate, double lateFee)
        : Vehicle(std::move(reg), rate, lateFee) {}
    std::string type() const override { return "Bicycle"; }
    double rentalCharge(int days) const override { return baseCharge(days); }
    std::string details() const override { return "pedal-powered"; }
};

class MotorVehicle : public Vehicle {
public:
    MotorVehicle(std::string reg, double rate, double lateFee, double energyLevel,
                 ServiceDate serviceDue)
        : Vehicle(std::move(reg), rate, lateFee), energyLevel_(energyLevel),
          serviceDue_(serviceDue) {}
    std::string details() const override {
        std::ostringstream out;
        out << "energy level " << energyLevel_ << "%, service due " << serviceDue_.str();
        return out.str();
    }

protected:
    double energyLevel() const { return energyLevel_; }
    std::string serviceDue() const { return serviceDue_.str(); }

private:
    double energyLevel_;
    ServiceDate serviceDue_;
};

class TwoWheeler final : public MotorVehicle {
public:
    TwoWheeler(std::string reg, double rate, double lateFee, double level,
               ServiceDate due, double fuelLitres, double perLitre)
        : MotorVehicle(std::move(reg), rate, lateFee, level, due),
          fuelLitres_(fuelLitres), perLitre_(perLitre) {}
    std::string type() const override { return "Two-wheeler"; }
    double rentalCharge(int days) const override {
        return baseCharge(days) + fuelLitres_ * perLitre_;
    }
    std::string details() const override {
        return MotorVehicle::details() + ", fuel charge " + std::to_string(fuelLitres_ * perLitre_);
    }

private:
    double fuelLitres_;
    double perLitre_;
};

class Car : public MotorVehicle {
public:
    Car(std::string reg, double rate, double lateFee, double level, ServiceDate due,
        int seats)
        : MotorVehicle(std::move(reg), rate, lateFee, level, due), seats_(seats) {}
    int seatingCapacity() const { return seats_; }

protected:
    std::string carDetails() const {
        return MotorVehicle::details() + ", seats " + std::to_string(seats_);
    }

private:
    int seats_;
};

class FuelCar final : public Car {
public:
    FuelCar(std::string reg, double rate, double lateFee, double level,
            ServiceDate due, int seats, double fuelLitres, double perLitre)
        : Car(std::move(reg), rate, lateFee, level, due, seats),
          fuelLitres_(fuelLitres), perLitre_(perLitre) {}
    std::string type() const override { return "Car"; }
    double rentalCharge(int days) const override {
        return baseCharge(days) + fuelLitres_ * perLitre_;
    }
    std::string details() const override {
        return carDetails() + ", fuel charge " + std::to_string(fuelLitres_ * perLitre_);
    }

private:
    double fuelLitres_;
    double perLitre_;
};

class ElectricCar final : public Car {
public:
    ElectricCar(std::string reg, double rate, double lateFee, double level,
                ServiceDate due, int seats, double batteryKwh,
                double consumedKwh, double pricePerKwh)
        : Car(std::move(reg), rate, lateFee, level, due, seats),
          batteryKwh_(batteryKwh), consumedKwh_(consumedKwh), pricePerKwh_(pricePerKwh) {}
    std::string type() const override { return "Electric car"; }
    double rentalCharge(int days) const override {
        return baseCharge(days) + consumedKwh_ * pricePerKwh_;
    }
    std::string details() const override {
        return carDetails() + ", battery " + std::to_string(batteryKwh_) + " kWh";
    }

private:
    double batteryKwh_;
    double consumedKwh_;
    double pricePerKwh_;
};

#endif
