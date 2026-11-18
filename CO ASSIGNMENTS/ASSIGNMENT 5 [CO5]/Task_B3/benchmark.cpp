#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>

enum class Kind { OP, IP, DayCare, Emergency };
struct Record { Kind kind; double a,b,c; int days; };
Record dataFor(std::size_t i) {
    switch (i % 4) {
    case 0: return {Kind::OP,100.0+(i%8)*50.0,200.0+(i%5)*20.0,0.0,0};
    case 1: return {Kind::IP,1800.0+(i%7)*100.0,3000.0+(i%9)*500.0,0.0,1+static_cast<int>(i%5)};
    case 2: return {Kind::DayCare,25000.0+(i%6)*1500.0,0.0,0.0,0};
    default:return {Kind::Emergency,1500.0+(i%9)*100.0,900.0+(i%4)*100.0,1.0+static_cast<double>(i%3),0};
    }
}
double recordTotal(const Record& r) noexcept {
    switch (r.kind) {
    case Kind::OP:return r.a+r.b;
    case Kind::IP:return r.days*r.a+r.b;
    case Kind::DayCare:return r.a;
    case Kind::Emergency:return r.a+r.b*r.c;
    }
    return 0.0;
}

struct BenchBill { virtual ~BenchBill()=default; virtual double total() const noexcept=0; };
struct BenchOP final : BenchBill { double x,y; BenchOP(double a,double b):x(a),y(b){} double total() const noexcept override{return x+y;} };
struct BenchIP final : BenchBill { int days; double rate,surgery; BenchIP(int d,double r,double s):days(d),rate(r),surgery(s){} double total() const noexcept override{return days*rate+surgery;} };
struct BenchDay final : BenchBill { double price; explicit BenchDay(double p):price(p){} double total() const noexcept override{return price;} };
struct BenchEmergency final : BenchBill { double base,rate,hours; BenchEmergency(double b,double r,double h):base(b),rate(r),hours(h){} double total() const noexcept override{return base+rate*hours;} };
struct OPData { double x,y; double total() const noexcept{return x+y;} };
struct IPData { int days; double rate,surgery; double total() const noexcept{return days*rate+surgery;} };
struct DayData { double price; double total() const noexcept{return price;} };
struct EmergencyData { double base,rate,hours; double total() const noexcept{return base+rate*hours;} };
using VariantBill=std::variant<OPData,IPData,DayData,EmergencyData>;

template<class Function>
double timeMs(Function&& f) {
    const auto start=std::chrono::steady_clock::now();
    f();
    const auto end=std::chrono::steady_clock::now();
    return std::chrono::duration<double,std::milli>(end-start).count();
}
volatile double sinkValue=0.0;
std::size_t repetitionsFor(std::size_t n) {
    if(n<=10)return 10000;
    if(n<=1000)return 1000;
    if(n<=1000000)return 5;
    return 2;
}

double runVirtual(std::size_t n, double& ms) {
    std::vector<BenchOP> op; std::vector<BenchIP> ip; std::vector<BenchDay> day; std::vector<BenchEmergency> emergency;
    op.reserve((n+3)/4);ip.reserve(n/4+1);day.reserve(n/4+1);emergency.reserve(n/4+1);
    std::vector<const BenchBill*> items;items.reserve(n);
    for(std::size_t i=0;i<n;++i) {
        const Record r=dataFor(i);
        switch(r.kind) {
        case Kind::OP:op.push_back({r.a,r.b});items.push_back(&op.back());break;
        case Kind::IP:ip.push_back({r.days,r.a,r.b});items.push_back(&ip.back());break;
        case Kind::DayCare:day.emplace_back(r.a);items.push_back(&day.back());break;
        case Kind::Emergency:emergency.push_back({r.a,r.b,r.c});items.push_back(&emergency.back());break;
        }
    }
    double result=0.0;
    const std::size_t reps=repetitionsFor(n);
    ms=timeMs([&]{for(std::size_t r=0;r<reps;++r){double sum=0;for(const BenchBill* b:items)sum+=b->total();sinkValue=sum;result=sum;}})/static_cast<double>(reps);
    return result;
}
double runSwitch(std::size_t n, double& ms) {
    std::vector<Record> items;items.reserve(n);for(std::size_t i=0;i<n;++i)items.push_back(dataFor(i));
    double result=0.0;
    const std::size_t reps=repetitionsFor(n);
    ms=timeMs([&]{for(std::size_t r=0;r<reps;++r){double sum=0;for(const Record& item:items)sum+=recordTotal(item);sinkValue=sum;result=sum;}})/static_cast<double>(reps);
    return result;
}
double runVariant(std::size_t n, double& ms) {
    std::vector<VariantBill> items;items.reserve(n);
    for(std::size_t i=0;i<n;++i) {
        const Record r=dataFor(i);
        switch(r.kind) {
        case Kind::OP:items.emplace_back(OPData{r.a,r.b});break;
        case Kind::IP:items.emplace_back(IPData{r.days,r.a,r.b});break;
        case Kind::DayCare:items.emplace_back(DayData{r.a});break;
        case Kind::Emergency:items.emplace_back(EmergencyData{r.a,r.b,r.c});break;
        }
    }
    double result=0.0;
    const std::size_t reps=repetitionsFor(n);
    ms=timeMs([&]{for(std::size_t r=0;r<reps;++r){double sum=0;for(const VariantBill& v:items)sum+=std::visit([](const auto& bill){return bill.total();},v);sinkValue=sum;result=sum;}})/static_cast<double>(reps);
    return result;
}

enum class Status { Ok, Negative, TooLarge };
Status validateReturn(double amount) noexcept {
    if(amount<0.0)return Status::Negative;
    if(amount>1.0e6)return Status::TooLarge;
    return Status::Ok;
}
void validateThrow(double amount) {
    if(amount<0.0)throw std::invalid_argument("negative");
    if(amount>1.0e6)throw std::out_of_range("too large");
}
struct ErrorTiming { double codeMs;double exceptionMs;std::size_t codeErrors;std::size_t exceptionErrors; };
ErrorTiming compareErrorModes(std::size_t n,int invalidPercent) {
    std::vector<double> claims(n,500.0);
    const std::size_t bad=n*static_cast<std::size_t>(invalidPercent)/100;
    for(std::size_t i=0;i<bad;++i)claims[i]=-1.0;
    std::size_t codeErrors=0,exceptionErrors=0;double sum=0.0;
    const double codeMs=timeMs([&]{for(double value:claims){const Status s=validateReturn(value);if(s!=Status::Ok)++codeErrors;else sum+=value;}sinkValue=sum;});
    sum=0.0;
    const double exceptionMs=timeMs([&]{for(double value:claims){try{validateThrow(value);sum+=value;}catch(const std::exception&){++exceptionErrors;}}sinkValue=sum;});
    return {codeMs,exceptionMs,codeErrors,exceptionErrors};
}

int main() {
    std::cout<<std::fixed<<std::setprecision(6);
    std::cout<<"B3 benchmark: identical deterministic bill fields; setup excluded from timed loops\n";
    std::cout<<"n,virtual_ms,switch_ms,variant_ms,totals_equal,passes\n";
    {
        double tv=0,ts=0,tn=0;
        const double v=runVirtual(10,tv),s=runSwitch(10,ts),q=runVariant(10,tn);
        const bool equal=std::abs(v-s)<0.01&&std::abs(s-q)<0.01;
        std::cout<<"10,"<<tv<<','<<ts<<','<<tn<<','<<(equal?"yes":"NO")<<','<<repetitionsFor(10)
                 <<" total="<<std::setprecision(2)<<v<<std::setprecision(6)<<"\n";
        assert(equal && std::abs(v-89940.0)<0.01);
    }
    for(std::size_t n:{1000u,1000000u,10000000u}) {
        double tv=0,ts=0,tn=0;
        const double v=runVirtual(n,tv),s=runSwitch(n,ts),q=runVariant(n,tn);
        const bool equal=std::abs(v-s)<0.01&&std::abs(s-q)<0.01;
        std::cout<<n<<','<<tv<<','<<ts<<','<<tn<<','<<(equal?"yes":"NO")<<','<<repetitionsFor(n)<<"\n";
        assert(equal);
    }
    std::cout<<"claims,invalid_percent,return_code_ms,exceptions_ms,code_errors,exception_errors\n";
    for(int percent:{0,1,50}) {
        const ErrorTiming e=compareErrorModes(1000000,percent);
        std::cout<<1000000<<','<<percent<<','<<e.codeMs<<','<<e.exceptionMs<<','<<e.codeErrors<<','<<e.exceptionErrors<<"\n";
        assert(e.codeErrors==e.exceptionErrors);
    }
}
