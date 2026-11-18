#include "hospital_billing.hpp"
#include <iostream>
#include <limits>
#include <memory>
#include <string>

using namespace hospital;

double approve(double billed, double cover) {
    if (billed < 0.0 || cover < 0.0) throw std::invalid_argument("negative amount");
    if (billed > cover) throw ClaimError("exceeds cover");
    return billed;
}
double settleClaim(double billed, double cover) {
    try { return approve(billed,cover); }
    catch (const ClaimError& e) { std::cout << "audit: " << e.what() << '\n'; throw; }
}
struct Tracked {
    std::string name;
    explicit Tracked(std::string n) : name(std::move(n)) {}
    ~Tracked() noexcept { std::cout << "destroy " << name << '\n'; }
};
template<class Function>
void runCase(const char* label, Function&& f) {
    std::cout << "case " << label << ": ";
    try { f(); std::cout << "ok\n"; }
    catch (const ClaimError& e) { std::cout << "ClaimError: " << e.what() << '\n'; }
    catch (const PackageError& e) { std::cout << "PackageError: " << e.what() << '\n'; }
    catch (const std::bad_alloc& e) { std::cout << "bad_alloc: " << e.what() << '\n'; }
    catch (const std::exception& e) { std::cout << "exception: " << e.what() << '\n'; }
    catch (...) { std::cout << "unknown failure\n"; }
}

int main() {
    std::cout << "B2 error-condition tests\n";
    runCase("negative charge", [] { OutPatientBill b("Ravi",-1,20); (void)b; });
    runCase("claim above cover", [] { (void)settleClaim(60000,50000); });
    runCase("unknown package", [] { DayCareBill b("Arun","NO-SUCH-CODE"); (void)b; });
    runCase("text instead of number", [] { (void)std::stod("not-a-number"); });
#ifdef CO5_ASAN_SAFE
    runCase("injected allocation failure under ASan", [] {
        // AddressSanitizer rejects impossible allocation sizes before throwing.
        // Inject bad_alloc here to verify the recovery/catch path under ASan.
        throw std::bad_alloc();
#else
    runCase("impossible allocation request", [] {
        volatile std::size_t requested=std::numeric_limits<std::size_t>::max();
        std::unique_ptr<char[]> memory(new char[requested]);
        (void)memory;
#endif
    });
    std::cout << "RAII rethrow path: ";
    try {
        Tracked patientRecord("patient-record");
        Tracked file("audit-file");
        throw ClaimError("forced failure");
    } catch (...) {
        std::cout << "top-level catch(...) recovered\n";
    }
    std::cout << "program continues after all cases\n";
}
