
//
// Created by qiu on 2021/12/1.
//

#include "LogStream.h"
#include<algorithm>

namespace liblog {
    const char digits[] = "9876543210123456789";
    const char *zero = digits + 9;
    const char digitsHex[] = "0123456789ABCDEF";


    template<typename T>
    int convert(char buf[], T value) {
        T i = value;
        char *p = buf;
        do {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0) {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);

        return static_cast<int>(p - buf);
    }

    template<typename T>
    int convertHEX(char buf[], T value) {
        uintptr_t i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 16);
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);

        *p = '\0';
        std::reverse(buf, p);

        return static_cast<int>(p - buf);
    }

    template<typename T>
    void LogStream::formatInterger(T v) {
        if (buffer_.avail() >= 32) {
            int len = convert(buffer_.current(), v);
            buffer_.add(len);
        }
    }

    LogStream::self &LogStream::operator<<(short v) {
        *this << static_cast<int>(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(unsigned short v) {
        *this << static_cast<unsigned int>(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(int v) {
        formatInterger(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(unsigned int v) {
        formatInterger(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(long v) {
        formatInterger(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(unsigned long v) {
        formatInterger(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(long long v) {
        formatInterger(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(unsigned long long v) {
        formatInterger(v);
        return *this;
    }

    LogStream::self &LogStream::operator<<(const void *p) {
        auto v = reinterpret_cast<int64_t>(p);
        if (buffer_.avail() >= 64) {
            char *buf = buffer_.current();
            buf[0] = '0';
            buf[1] = 'x';
            int len = convertHEX(buf + 2, v);
            buffer_.add(len + 2);
        }
        return *this;
    }
}
