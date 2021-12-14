//
// Created by qiu on 2021/12/11.
//

#include "Timestamp.h"
#include<time.h>
#include<sys/time.h>
#include<chrono>
using namespace liblog;
Timestamp Timestamp::now() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    //auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();//微秒数,int64_t
    std::chrono::microseconds nowin_us=std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return Timestamp(nowin_us.count());
//    struct timeval tm;
//    gettimeofday(&tm,nullptr);
//    int64_t seconds=tm.tv_sec;
//    return Timestamp(seconds * 1000000 + tm.tv_usec);
}

std::string Timestamp::toformattedString(bool showMicroseconds) {
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / 1000000);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    if (showMicroseconds)
    {
        int microseconds = static_cast<int>(microSecondsSinceEpoch_ % 1000000);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                 microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}
