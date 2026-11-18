#include "hospital_billing.hpp"
#include <iostream>
#include <memory>
#include <vector>

// New bill type: added without modifying Bill, existing bills, or BillingSystem.
class TelemedicineBill final : public hospital::Bill {
    double consultation_;
    double remoteTest_;
public:
    TelemedicineBill(std::string patient,double consultation,double remoteTest)
        : Bill(std::move(patient)), consultation_(hospital::checkedCharge(consultation,"consultation")),
          remoteTest_(hospital::checkedCharge(remoteTest,"remote test")) {}
    double total() const noexcept override { return consultation_+remoteTest_; }
    std::string_view type() const noexcept override { return "TELEMEDICINE"; }
};

// New payer type: another independent implementation of the existing Payer interface.
class CorporateTieUp final : public hospital::Payer {
    double coveragePercent_;
    double limit_;
public:
    CorporateTieUp(double percentage,double limit):coveragePercent_(percentage),limit_(hospital::checkedCharge(limit,"corporate limit")) {
        if(percentage<0||percentage>100)throw hospital::ClaimError("corporate coverage must be 0..100");
    }
    double amountCovered(const hospital::Bill& bill) const override {
        return std::min(limit_,bill.total()*coveragePercent_/100.0);
    }
    std::string_view name() const noexcept override { return "CorporateTieUp"; }
};

struct ExtensionGuard {
    ~ExtensionGuard() noexcept { std::cout<<"extension resource released\n"; }
};

int main() {
    using namespace hospital;
    BillingSystem system;
    system.add(std::make_unique<OutPatientBill>("Ravi",500,1200));
    system.add(std::make_unique<InPatientBill>("Meena",3,2500,40000));
    system.add(std::make_unique<TelemedicineBill>("Hitesh",800,250));
    CorporateTieUp corporate(90,50000);
    for(const auto& bill:system.bills()) {
        const auto result=settle(*bill,corporate);
        std::cout<<bill->type()<<' '<<bill->patient()<<" total="<<result.billTotal
                 <<" covered="<<result.covered<<" due="<<result.patientDue<<'\n';
    }
    try {
        ExtensionGuard guard;
        if (60000>50000) throw ClaimError("exceeds cover");
    } catch (const ClaimError& error) {
        std::cout<<"caught extension test: "<<error.what()<<'\n';
    }
}
