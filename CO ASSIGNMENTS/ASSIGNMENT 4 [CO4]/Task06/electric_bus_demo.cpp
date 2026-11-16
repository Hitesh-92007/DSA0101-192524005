#include "electric_bus_extension.hpp"

#include <array>
#include <iomanip>
#include <iostream>

int main() {
    const ServiceDate due{2026, 12, 1};
    Bicycle bicycle("BIKE-02", 100, 15);
    ElectricBus bus("BUS-01", 5000, 400, 65, due, 40, 300, 45, 15);
    std::array<Vehicle*, 2> fleet{&bicycle, &bus};

    std::cout << "Fleet extended with ElectricBus without editing existing vehicle classes\n";
    for (Vehicle* vehicle : fleet) {
        std::cout << vehicle->listing() << '\n';
        vehicle->rent(1);
    }
    std::cout << std::fixed << std::setprecision(2)
              << "BUS-01 return charge with 1 late day: " << bus.returnVehicle(1) << '\n';
}
