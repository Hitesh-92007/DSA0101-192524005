# Task 6 Design Challenge Vehicle Rental Fleet

## Stage 1 Requirements

The fleet has bicycles, two-wheelers, fuel cars and electric cars. Every vehicle stores a registration number and daily rate, can be rented and returned, reports its availability, and computes a rental charge. Motor vehicles also store an energy level and service due date. Cars have seating capacity. Electric cars store battery capacity and bill consumed energy per kWh. A new vehicle type must be addable through the common interface without modifying the existing vehicle classes.

Testable requirements include: each available vehicle can be rented for a positive number of days; an already rented vehicle rejects another rental; the return total includes the booked period, any late days and the late fee; the returned vehicle becomes available; the listing uses dynamic type names; and each derived rental calculation uses its own fuel or electricity formula. The extension demo adds an electric bus and calls it through a `Vehicle*`.

## Stage 2 and 3 Hierarchy and decisions

```text
Vehicle (abstract: rent, return, rentalCharge, type, details)
├── Bicycle
└── MotorVehicle (energy level, ServiceDate member)
    ├── TwoWheeler
    └── Car (seating capacity, abstract)
        ├── FuelCar
        └── ElectricCar (battery capacity and kWh charge)

ElectricBus extends MotorVehicle in electric_bus_extension.hpp
```

The hierarchy uses single inheritance, hierarchical inheritance from `Vehicle`/`MotorVehicle`, and multilevel inheritance along `Vehicle -> MotorVehicle -> Car -> ElectricCar`. `ServiceDate` is a member value of `MotorVehicle`. `Vehicle` is abstract because its type-specific charge, name and details are pure virtual. All inheritance is public for these is-a relationships. The fleet is an array of non-owning base-class pointers to stack objects, which demonstrates runtime polymorphism without manual deletion. The base destructor is virtual.

## Stage 4 Design and pseudocode

The class declarations and implementations are in `vehicle_rental.hpp`. `Task06/pseudocode.md` describes rent, return and list operations. `returnVehicle()` computes the charge through the virtual function, so a pointer to the base applies the correct fuel or kWh formula.

## Stage 5 Demonstration

`vehicle_rental.cpp` lists, rents and returns all four standard types; the return is two days late. `electric_bus_demo.cpp` adds `ElectricBus` in a separate extension header and uses it in a `Vehicle*` array alongside a bicycle. The existing class declarations are not changed. `RUN_OUTPUT.txt` contains both runs.

## Alternative single-class switch

A single `Vehicle` class with a type code could store the union of bicycle, fuel and battery fields and switch in `charge()`. Both a type switch and virtual dispatch are O(1) per charge call; processing n vehicles is O(n). The single class reserves or manages fields that many object types do not use. A new type would modify the type code, validation/construction, the charge switch and often listing logic. With the abstract base, a new derived type implements the operations and is inserted into the fleet using the existing interface. Virtual dispatch adds a vtable pointer per polymorphic object; the switch version avoids that pointer but may have larger unused object storage. Exact `sizeof` depends on the compiler and layout.
