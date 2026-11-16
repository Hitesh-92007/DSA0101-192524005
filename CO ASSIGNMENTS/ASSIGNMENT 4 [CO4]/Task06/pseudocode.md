# Task 6 rental and return pseudocode

```text
PROCEDURE Rent(vehicle, requestedDays)
    IF requestedDays < 1 OR vehicle is already rented THEN
        RETURN false
    END IF
    vehicle.bookedDays <- requestedDays
    vehicle.rented <- true
    RETURN true
END PROCEDURE

PROCEDURE ReturnVehicle(vehicle, lateDays)
    IF vehicle is not rented OR lateDays < 0 THEN
        RETURN invalid-charge
    END IF
    totalDays <- vehicle.bookedDays + lateDays
    total <- vehicle.rentalCharge(totalDays)
    total <- total + lateDays * vehicle.lateFeePerDay
    vehicle.rented <- false
    vehicle.bookedDays <- 0
    RETURN total
END PROCEDURE

PROCEDURE ListFleet(vehiclePointers[0..n-1])
    FOR each pointer in vehiclePointers
        PRINT pointer.type, registration, availability, and type-specific details
    END FOR
END PROCEDURE
```
