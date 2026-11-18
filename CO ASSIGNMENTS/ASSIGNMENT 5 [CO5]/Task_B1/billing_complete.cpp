#include "hospital_billing.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    using namespace hospital;
    std::cout << std::fixed << std::setprecision(2);
    BillingSystem system;
    system.add(std::make_unique<OutPatientBill>("Ravi",500,1200));
    system.add(std::make_unique<InPatientBill>("Meena",3,2500,40000));
    system.add(std::make_unique<DayCareBill>("Arun","DC-101"));
    system.add(std::make_unique<EmergencyBill>("Nila",2500,1200,2));
    SelfPay self; Insurance insurance(50000,20); GovernmentScheme government;
    const std::vector<const Payer*> payers{&self,&insurance,&government};
    double daily=0.0;
    for (const auto& bill : system.bills()) {
        bill->print(); daily += bill->total();
        for (const Payer* payer : payers) {
            const auto result=settle(*bill,*payer);
            std::cout << "  " << payer->name() << " covered=" << result.covered
                      << " patient_due=" << result.patientDue << '\n';
        }
    }
    std::cout << "Daily total=" << daily << '\n';
}
