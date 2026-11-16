# CO4 Assignment 4 Designing Reusable Class Hierarchies

**Student:** Hitesh C  
**Register number:** 192524005  
**Course:** Object Oriented Programming with C++ (DSA01)  
**Repository:** https://github.com/Hitesh-92007/DSA0101-192524005/tree/main/CO%20ASSIGNMENTS/ASSIGNMENT%204%20%5BCO4%5D

This report covers all six tasks: payroll hierarchy design, university relationships, constructor and member-class behavior, polymorphic account defects, Cart pointers and `this`, and a reusable vehicle-rental fleet. The programs were built with C++17 and `-Wall -Wextra`. This folder was prepared with coding assistance. The assignment requires each student to write predictions before running the code and execute every task individually. The included prediction notes are references, not a record of the student's independent predictions. Write your own predictions before rerunning the programs locally, verify the outputs, and add screenshots from those runs. The current output images are formatted from saved logs, not raw terminal screenshots.

The available driver is the `g++` command supplied by LLVM-MinGW; `g++ --version` reports Clang 23.1.1. The AddressSanitizer runtime used here does not support LeakSanitizer. The defective account program therefore includes destructor messages and a live-allocation counter; no sanitizer finding was emitted in this run.

## Task 1 Class Design Review

The original payroll classes repeat name and ID, expose their state publicly, use inconsistent names for the same information, retain an unused da field and embed 0.42 as a magic number. Their different data and pay methods also make a common payroll loop difficult.

The repaired abstract Staff class owns name and ID and declares virtual pay(). Professor stores basic pay and research allowance, LabAssistant stores basic pay, and GuestFaculty stores hours and rate. Each derived class overrides the same interface. The payroll vector owns Staff pointers and processes every type in one loop.

For the sample values, Professor Anita earns 81,000.00, LabAssistant Ravi earns 42,600.00, and GuestFaculty Meera earns 30,000.00. A single class with a type switch also has O(1) pay-call time, but it reserves unrelated fields and the new category changes the switch and its type/creation logic. The hierarchy adds a derived class while leaving the existing loop unchanged.

| Issue | Effect | Repair |
|---|---|---|
| Repeated name and ID | Fields and maintenance are duplicated | Move common identity into Staff |
| Public fields | Callers can create invalid values | Keep state private |
| Inconsistent names | nm/code and name/id mean the same thing | Use one shared naming scheme |
| Unused da member | Stored state never affects pay | Remove unused data |
| Magic number 0.42 | Policy is hidden in formula code | Use a named DA rate constant |
| Different pay interfaces | One loop cannot process all staff uniformly | Declare virtual pay() in Staff |
| Adding a staff type | Clients need category-specific branches | Add a derived type through the common interface |

![Payroll class UML](Task01/UML.png)

![Task 1 compiled program output](Task01/output_capture.png)

## Task 2 From Requirement to Hierarchy

Student and Staff are each Persons. TeachingAssistant is both a Student and Staff member, forming a diamond through Person. ResearchScholar is a Student with additional fellowship data. Student and Staff use public virtual inheritance so TeachingAssistant contains one shared Person subobject.

The constructor prediction was Person Student Staff TA. The run matched it. Removing virtual printed Person Student Person Staff TA; this constructor-only program compiles without a diagnostic. Reading ta.name through the non-virtual diamond is ambiguous, and the separate expected-failure compile check records that compiler error.

For composition, TeachingAssistant contains separate Student and Staff objects. Each member has its own Person data, so name access is explicit through student.person or staff.person. On the compiler used, sizeof(Student)=88, sizeof(Staff)=88 and sizeof(TeachingAssistant)=176 bytes. These sizes are ABI-specific. Field lookup is O(1) in both designs.

| Relationship | Kind | Inheritance form |
|---|---|---|
| Student derives from Person | Is-a | Single |
| Staff derives from Person | Is-a | Single |
| TeachingAssistant derives from Student and Staff | Is-a | Multiple; hybrid with shared Person |
| ResearchScholar derives from Student | Is-a | Multilevel |
| Student stores programme and CGPA | Has-a/value data | Member data |
| Staff stores department and salary | Has-a/value data | Member data |

| Person member | public inheritance | protected inheritance | private inheritance |
|---|---|---|---|
| public | public | protected | private |
| protected | protected | protected | private |
| private | not directly accessible | not directly accessible | not directly accessible |

![People hierarchy UML](Task02/UML.png)

![Task 2 constructor outputs](Task02/output_capture.png)

**Expected ambiguous-access diagnostic:** see `Task02/EXPECTED_DIAGNOSTIC.txt`.

## Task 3 Constructors and Member Classes

Address is a value member of Person, and Course is a value member of Student. Constructors pass values through initializer lists. When TeachingAssistant is the most-derived type, its initializer directly constructs the shared virtual Person base; Student and Staff do not construct a second copy.

Observed construction order is virtual bases first, then direct bases in declaration order, then members in declaration order, then the constructor body. Destruction reverses that order. The supplied Engine/Vehicle/Car program printed `Vehicle Engine Car | ~Car ~Engine ~Vehicle`, matching the base/member construction and reverse destruction rules. The table gives the complete logged sequence. The virtual-base TeachingAssistant occupies 24 bytes and the non-virtual comparison occupies 2 bytes on the LLVM-MinGW ABI used; exact sizes vary by implementation.

The completed Student member-class program prints `192311001 Kavya (19) Salem-636001`. Reversing the initializer list produces a -Wreorder-ctor warning, but members still initialize in declaration order and print `1 2`. A pointer-owned Address adds an allocation and ownership work; unique_ptr is safer than raw new/delete, while a value member manages its lifetime automatically.

| Object | Construction | Destruction |
|---|---|---|
| Student | Address, Person, Course, Student | Student, Course, Person, Address |
| Staff | Address, Person, Staff | Staff, Person, Address |
| TeachingAssistant | Address, Person, Course, Student, Staff, TeachingAssistant | TeachingAssistant, Staff, Student, Course, Person, Address |

![Member-class and virtual-base UML](Task03/UML.png)

![Task 3 constructor, warning and complete-code outputs](Task03/output_capture.png)

## Task 4 Pointers to Derived Classes Forensics

The defective Account example has a non-virtual rate function, a non-virtual destructor, raw derived storage and a sliced Account value. The rate through Account* is 3.5 because the function is not virtual. Deleting a SeniorAccount through a base pointer is undefined behavior; its derived destructor is skipped on this implementation, leaving its 100-double history allocation outstanding.

The defective source was built with AddressSanitizer. This Windows runtime does not support LeakSanitizer, and it emitted no sanitizer finding in this run. The program's destructor log and explicit allocation counter show two base destructor calls, no SeniorAccount destructor during base-pointer deletion, and one outstanding history buffer. This is direct runtime evidence, not a LeakSanitizer report.

The repair makes rate virtual, uses override, gives Account a virtual destructor and stores owning objects in vector<unique_ptr<Account>>. The corrected run prints 3.5 and 4.0 and releases the senior history. In the required prediction program, the values are 3.5 4 3.5: static call, virtual call, then sliced Account copy. Processing n accounts remains O(n); unique_ptr prevents ownership leaks but does not by itself make a non-virtual destructor safe or stop slicing.

| Defect | Root cause | Repair |
|---|---|---|
| Senior rate is not selected | rate() is non-virtual | virtual rate() and override |
| Derived cleanup skipped | Base destructor is non-virtual | virtual Account destructor |
| History buffer remains allocated | SeniorAccount destructor does not run through Account* | virtual destruction and RAII |
| Derived data disappears | Account copy slices SeniorAccount | Keep polymorphic objects behind pointers/references |
| Manual ownership is fragile | Raw owning Account* values | vector<unique_ptr<Account>> |

| Expression | Binding rule | Output |
|---|---|---|
| p->rate() | Non-virtual; uses static Account type | 3.5 |
| p->vrate() | Virtual; uses dynamic SeniorAccount type | 4 |
| copy.vrate() | Account value was sliced | 3.5 |

![Task 4 account outputs](Task04/output_capture.png)

**AddressSanitizer build and destructor-tracking record:** see `Task04/SANITIZER_RESULTS.txt`.

## Task 5 Pointers to Objects and the this Pointer

The completed code returns Cart& from add() and setDiscount(), allowing calls to chain on the same object. Inside a member function, this points to the current cart and *this is that cart as a reference. The expression this->discount_ separates the data member from the discount parameter. Returning Cart by value would copy the object and continue the chain on the temporary copy.

The supplied code prints 324.00: 60 + 120 + 180 is 360, and a 10 percent discount leaves 324. The second program demonstrates named-object, pointer, reference and dynamically allocated access. It scans an array of Cart pointers in O(n) time and O(1) extra space, returning the 900.00 cart as the largest.

A vector<Cart> also takes O(n) for the search. It stores full objects; an array of pointers stores pointers while the objects reside elsewhere. A pointer or reference to a vector element can be invalidated when growth reallocates the vector. vector<unique_ptr<Cart>> keeps separately allocated Cart addresses stable as the pointer vector grows.

| Access style | Example | Lifetime / safety |
|---|---|---|
| Named object | cart.add(...) / cart.total() | Automatic scope lifetime; safest for local use |
| Pointer | pointer->add(...) | Non-owning; check null and keep target alive |
| Reference | Cart& ref = cart | Alias to a live object; cannot be reseated |
| Dynamic pointer | new Cart / delete | Manual lifetime; use unique_ptr in production |

![Task 5 completed code and pointer-array output](Task05/output_capture.png)

## Task 6 Vehicle Rental Fleet

Vehicle is abstract and stores registration, daily rate, late-fee rate and rental state. Bicycle derives directly. MotorVehicle adds energy level and a ServiceDate member. TwoWheeler and Car derive from MotorVehicle; Car is abstract and holds seating capacity. FuelCar and ElectricCar implement different energy charges. ElectricBus is added in a separate extension header without changing the existing classes.

Rent accepts a positive day count only when the vehicle is available. Return computes the virtual rentalCharge for booked plus late days, adds a per-day late fee and resets the status. The fleet is an array of Vehicle pointers, so listing and charging use dynamic dispatch. Rental and return work is O(1) per item; listing or processing n fleet entries is O(n). Each vehicle stores a constant number of fields, so its per-object storage is O(1).

With one booked day and two late days, the standard sample totals are Bicycle 330.00, Two-wheeler 1000.00, Car 5800.00 and Electric car 7116.00. The added ElectricBus is listed through Vehicle* and returns for 11075.00 after one late day. A type-code switch also costs O(1) per charge, but adding a type changes the switch and often validation/listing; the hierarchy adds a derived implementation.

| Check | Expected behavior | Observed result |
|---|---|---|
| Rent available item | Positive days mark it rented | All four standard vehicles rented |
| Reject invalid/already rented request | Return false | Guard is implemented in Vehicle::rent |
| Return with two late days | Charge extended days plus late fee | 330.00, 1000.00, 5800.00, 7116.00 |
| List after return | Status returns to Available | All four listed as Available |
| Add electric bus | Use new derived type through Vehicle* | BUS-01 extension ran; return charge 11075.00 |

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
        PRINT type, registration, availability, and type-specific details
    END FOR
END PROCEDURE
```

![Vehicle class hierarchy UML](Task06/UML.png)

![Task 6 vehicle rental and ElectricBus extension output](Task06/output_capture.png)

## Build and verification summary

Seventeen intended programs compiled and ran. The one separate non-virtual `ta.name` source was expected to fail compilation because the name is ambiguous. The initializer-order demonstration produced the expected `-Wreorder-ctor` warning. The Task 3 prediction program printed `Vehicle Engine Car | ~Car ~Engine ~Vehicle`; its completed member-class program printed `192311001 Kavya (19) Salem-636001`. The Task 4 dispatch program printed `3.5 4 3.5`; the completed Task 5 program printed `324.00`.

The sanitizer-enabled defective account program exited successfully with leak detection disabled. It printed one outstanding senior history buffer after base-pointer deletion. LeakSanitizer was unavailable in this Windows runtime, so this is not reported as a sanitizer leak finding.

Build logs, expected compiler diagnostics, predictions and task outputs are saved in their task folders. The assignment brief leaves its submission date blank. Before submission, write your own predictions, run all programs on your computer, check the results and add raw screenshots from those runs, as required by the assignment.
