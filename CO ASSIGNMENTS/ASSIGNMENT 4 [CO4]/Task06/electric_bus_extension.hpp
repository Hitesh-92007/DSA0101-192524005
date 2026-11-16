#ifndef CO4_ELECTRIC_BUS_EXTENSION_HPP
#define CO4_ELECTRIC_BUS_EXTENSION_HPP

#include "vehicle_rental.hpp"

class ElectricBus final : public MotorVehicle {
public:
    ElectricBus(std::string reg, double rate, double lateFee, double level,
                ServiceDate due, int passengerCapacity, double batteryKwh,
                double consumedKwh, double pricePerKwh)
        : MotorVehicle(std::move(reg), rate, lateFee, level, due),
          passengerCapacity_(passengerCapacity), batteryKwh_(batteryKwh),
          consumedKwh_(consumedKwh), pricePerKwh_(pricePerKwh) {}

    std::string type() const override { return "Electric bus"; }
    double rentalCharge(int days) const override {
        return baseCharge(days) + consumedKwh_ * pricePerKwh_;
    }
    std::string details() const override {
        return MotorVehicle::details() + ", capacity " + std::to_string(passengerCapacity_) +
               ", battery " + std::to_string(batteryKwh_) + " kWh";
    }

private:
    int passengerCapacity_;
    double batteryKwh_;
    double consumedKwh_;
    double pricePerKwh_;
};

#endif
