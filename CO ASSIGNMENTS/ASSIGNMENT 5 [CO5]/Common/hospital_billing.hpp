#ifndef HOSPITAL_BILLING_HPP
#define HOSPITAL_BILLING_HPP

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace hospital {

class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
class NegativeChargeError : public BillingError {
public:
    using BillingError::BillingError;
};
class MissingPatientError : public BillingError {
public:
    using BillingError::BillingError;
};
class PackageError : public BillingError {
public:
    using BillingError::BillingError;
};
class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};

inline double checkedCharge(double value, std::string_view field)
{
    if (!std::isfinite(value) || value < 0.0)
        throw NegativeChargeError(std::string(field) + " must be a finite non-negative amount");
    return value;
}

class Bill {
protected:
    std::string patient_;
    explicit Bill(std::string patient) : patient_(std::move(patient))
    {
        if (patient_.empty()) throw MissingPatientError("patient record is missing");
    }
public:
    virtual ~Bill() noexcept = default;
    virtual double total() const noexcept = 0;
    virtual std::string_view type() const noexcept = 0;
    std::string_view patient() const noexcept { return patient_; }
    void print() const { // intentionally non-virtual: calls virtual functions below
        std::cout << type() << ' ' << patient() << ": " << total() << '\n';
    }
};

class OutPatientBill final : public Bill {
    double consultation_;
    double tests_;
public:
    OutPatientBill(std::string p, double consultation, double tests)
        : Bill(std::move(p)), consultation_(checkedCharge(consultation, "consultation")),
          tests_(checkedCharge(tests, "tests")) {}
    double total() const noexcept override { return consultation_ + tests_; }
    std::string_view type() const noexcept override { return "OP"; }
};

class InPatientBill final : public Bill {
    int days_;
    double perDay_;
    double surgery_;
public:
    InPatientBill(std::string p, int days, double perDay, double surgery)
        : Bill(std::move(p)), days_(days), perDay_(checkedCharge(perDay, "room rate")),
          surgery_(checkedCharge(surgery, "surgery"))
    {
        if (days_ <= 0) throw NegativeChargeError("in-patient days must be positive");
    }
    double total() const noexcept override { return days_ * perDay_ + surgery_; }
    std::string_view type() const noexcept override { return "IP"; }
};

class DayCareBill final : public Bill {
    std::string packageCode_;
    double packagePrice_;
    static double priceFor(std::string_view code)
    {
        static const std::map<std::string, double> packages{
            {"DC-101", 32000.0}, {"DC-202", 45000.0}};
        const auto it = packages.find(std::string(code));
        if (it == packages.end()) throw PackageError("unknown package code: " + std::string(code));
        return it->second;
    }
public:
    DayCareBill(std::string p, std::string code)
        : Bill(std::move(p)), packageCode_(std::move(code)), packagePrice_(priceFor(packageCode_)) {}
    double total() const noexcept override { return packagePrice_; }
    std::string_view type() const noexcept override { return "DAYCARE"; }
    std::string_view packageCode() const noexcept { return packageCode_; }
};

class EmergencyBill final : public Bill {
    double base_;
    double hourlyRate_;
    double hours_;
public:
    EmergencyBill(std::string p, double base, double hourlyRate, double hours)
        : Bill(std::move(p)), base_(checkedCharge(base, "emergency base charge")),
          hourlyRate_(checkedCharge(hourlyRate, "emergency hourly rate")),
          hours_(checkedCharge(hours, "emergency hours")) {}
    double total() const noexcept override { return base_ + hourlyRate_ * hours_; }
    std::string_view type() const noexcept override { return "EMERGENCY"; }
};

class Payer {
public:
    virtual ~Payer() noexcept = default;
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual std::string_view name() const noexcept = 0;
};

class SelfPay final : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0.0; }
    std::string_view name() const noexcept override { return "SelfPay"; }
};

class Insurance final : public Payer {
    double coverLimit_;
    double copayPercent_;
public:
    Insurance(double coverLimit, double copayPercent)
        : coverLimit_(checkedCharge(coverLimit, "cover limit")), copayPercent_(copayPercent)
    {
        if (!std::isfinite(copayPercent_) || copayPercent_ < 0.0 || copayPercent_ > 100.0)
            throw ClaimError("co-payment percentage must be between 0 and 100");
    }
    double amountCovered(const Bill& bill) const override
    {
        return std::min(coverLimit_, bill.total() * (100.0 - copayPercent_) / 100.0);
    }
    std::string_view name() const noexcept override { return "Insurance"; }
};

class GovernmentScheme final : public Payer {
    std::map<std::string, double> packageRates_;
public:
    GovernmentScheme() : packageRates_{{"OP",1500.0},{"IP",40000.0},
                                      {"DAYCARE",30000.0},{"EMERGENCY",3500.0}} {}
    double amountCovered(const Bill& bill) const override
    {
        const auto it = packageRates_.find(std::string(bill.type()));
        if (it == packageRates_.end()) throw PackageError("government package rate not found");
        return std::min(bill.total(), it->second);
    }
    std::string_view name() const noexcept override { return "GovernmentScheme"; }
};

struct Settlement { double billTotal; double covered; double patientDue; };

inline Settlement settle(const Bill& bill, const Payer& payer)
{
    const double total = bill.total();
    const double covered = payer.amountCovered(bill);
    if (!std::isfinite(covered) || covered < 0.0 || covered > total)
        throw ClaimError("payer returned an invalid covered amount");
    return {total, covered, total - covered};
}

class BillingSystem {
    std::vector<std::unique_ptr<Bill>> bills_;
public:
    void add(std::unique_ptr<Bill> bill) {
        if (!bill) throw BillingError("cannot add a null bill");
        bills_.push_back(std::move(bill));
    }
    double dailyTotal() const noexcept {
        double sum=0.0; for (const auto& bill : bills_) sum += bill->total(); return sum;
    }
    const std::vector<std::unique_ptr<Bill>>& bills() const noexcept { return bills_; }
};

} // namespace hospital
#endif
