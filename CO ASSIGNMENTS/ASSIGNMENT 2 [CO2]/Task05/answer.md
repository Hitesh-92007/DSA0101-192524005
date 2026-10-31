# Q5 - Static members and arrays of objects

## Token prediction and result

`Token::next` starts at 100. Constructing `counter[3]` runs the constructor three times and gives IDs 101, 102, and 103. Constructing `extra` runs it a fourth time and gives ID 104. Therefore the supplied expression prints:

```text
101 103 104 4
```

The static `issued` counter belongs to the class, so all `BusTicket` objects share it. It is defined once outside the class as `int BusTicket::issued = 0;`. The static `count()` function can be called as `BusTicket::count()` and does not need a ticket object. A static function has no `this` pointer and cannot directly access a non-static member such as one ticket's `seat` or `name`.

## Capacity test

The TODO logic increments `issued` and gives the ticket that seat only while `issued < CAPACITY`. The four-object array therefore assigns seats 1, 2, and 3; the fourth ticket is placed on the waiting list; the count remains 3. The program also calls a ticket-creation function twice after capacity is reached. Both later tickets wait, and the shared count remains 3.

Objects in an array are constructed in element order. Objects created in a loop or in another function still use the same static counter; leaving the function does not reset the static member. This design assumes one bus. If two buses operate at once, one static counter would mix their capacity. A `TicketOffice` object per bus, passed by reference to each ticket, would keep the counts independent. A global counter has O(1) time and storage too, but exposes state outside the class and is harder to manage for two offices. The static counter is encapsulated but still shared across all buses of this class.

The source is `q5_static_bus_tickets.cpp`; the run transcript is `RUN_OUTPUT.txt`.
