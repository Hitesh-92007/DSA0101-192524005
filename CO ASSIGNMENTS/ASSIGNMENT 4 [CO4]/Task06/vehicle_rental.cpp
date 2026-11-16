#include "vehicle_rental.hpp"

#include <array>
#include <iomanip>
#include <iostream>

int main() {
    const ServiceDate due{2026, 12, 1};
    Bicycle bicycle("BIKE-01", 100, 15);
    TwoWheeler scooter("SCOOT-01", 250, 25, 75, due, 2, 100);
    FuelCar car("CAR-01", 1500, 100, 60, due, 4, 10, 110);
    ElectricCar ev("EV-01", 2200, 150, 80, due, 5, 75, 18, 12);
    std::array<Vehicle*, 4> fleet{&bicycle, &scooter, &car, &ev};

    std::cout << "Fleet before rental\n";
    for (const Vehicle* vehicle : fleet) std::cout << vehicle->listing() << '\n';

    std::cout << "\nRent each vehicle for 1 day\n";
    for (Vehicle* vehicle : fleet) {
        std::cout << vehicle->registration() << ": "
                  << (vehicle->rent(1) ? "rented" : "not available") << '\n';
    }

    std::cout << "\nReturn after 2 late days (includes extended rental and late fee)\n";
    std::cout << std::fixed << std::setprecision(2);
    for (Vehicle* vehicle : fleet) {
        std::cout << vehicle->registration() << " total charge: "
                  << vehicle->returnVehicle(2) << '\n';
    }

    std::cout << "\nFleet after return\n";
    for (const Vehicle* vehicle : fleet) std::cout << vehicle->listing() << '\n';
}
