#include <iostream>

struct Person { char record[1]{}; };
struct Student : Person {};
struct Staff : Person {};
struct TeachingAssistant : Student, Staff {};

struct VirtualStudent : virtual Person {};
struct VirtualStaff : virtual Person {};
struct VirtualTeachingAssistant : VirtualStudent, VirtualStaff {};

int main() {
    std::cout << "Non-virtual TeachingAssistant size: " << sizeof(TeachingAssistant) << '\n'
              << "Virtual-base TeachingAssistant size: " << sizeof(VirtualTeachingAssistant) << '\n';
}
