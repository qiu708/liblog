
#ifndef LIBLOG_TIMESTAMP_H
#define LIBLOG_TIMESTAMP_H

#include<stdint.h>
#include<string>

namespace liblog{
class Timestamp {
private: int64_t microSecondsSinceEpoch_;
public:
    Timestamp():microSecondsSinceEpoch_(0){};
    explicit Timestamp(int64_t t):microSecondsSinceEpoch_(t){};
    static Timestamp now();
    int64_t microSecondsSinceEpoch() const
    {
        return microSecondsSinceEpoch_;
    }
    bool operator<(Timestamp r) const{
        return this->microSecondsSinceEpoch_<r.microSecondsSinceEpoch_;
    }
    bool operator==(Timestamp r) const{
        return this->microSecondsSinceEpoch_==r.microSecondsSinceEpoch_;
    }
    std::string toformattedString(bool showMicroseconds=false);
    Timestamp addTime(double seconds)
    {
        return Timestamp(this->microSecondsSinceEpoch_+1000000*seconds);
    }
    static const int kMicroSecondsPerSecond=1000*1000;
};

}




#endif
