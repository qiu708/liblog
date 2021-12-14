//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_LOGSTREAM_H
#define LIBLOG_LOGSTREAM_H

#include <cstring>
#include<string>
#include<iostream>
#include"noncopyable.h"
/*
 * 定义缓冲区类FixedBuffer,仅存放数据,不负责输入输出
 * LogStream类，重载<<运算符，将输入放入FixedBuffer
 * */

    namespace liblog {
        template<int SIZE>
        class FixedBuffer : noncopyable {

        public:
            FixedBuffer() : cur(data_) {}

            void append(const char *buf, int len) {
                if (avail() >= len) {
                    memcpy(cur, buf, len);
                    cur += len;
                }
            }

            int avail() { return static_cast<int>(end() - cur); }

            [[nodiscard]] int bufferlength() const {
                return this->cur - this->data_;
            }

            [[nodiscard]] const char *data() const { return data_; }

            void bzero() { memset(data_, 0, SIZE); }

            void reset() { cur = data_; }

            void add(int size) {
                cur += size;
            }

            char *current() { return cur; }

            std::string toString() { return std::string(data_, bufferlength()); }

        private:
            char data_[SIZE]{};
            char *cur;

            char *end() { return data_ + sizeof(data_); }
        };

        class LogStream {
        public:
            typedef FixedBuffer<4000> Buffer;
            typedef LogStream self;
        private:
            Buffer buffer_;
        public:
            self &operator<<(bool v) {
                buffer_.append(v ? "1" : "0", 1);
                return *this;
            }

            self &operator<<(short);

            self &operator<<(unsigned short);

            self &operator<<(int);

            self &operator<<(unsigned int);

            self &operator<<(long);

            self &operator<<(unsigned long);

            self &operator<<(long long);

            self &operator<<(unsigned long long);

            self &operator<<(const void *);

            self &operator<<(float v) {
                *this << static_cast<double>(v);
                return *this;
            }

            self &operator<<(double);

            self &operator<<(char v) {
                buffer_.append(&v, 1);
                return *this;
            }

            self &operator<<(const char *str) {
                if (str) {
                    buffer_.append(str, static_cast<int>(strlen(str)));
                } else {
                    buffer_.append("(null)", 6);
                }
                return *this;
            }

            self &operator<<(const std::string &v) {
                buffer_.append(v.c_str(), static_cast<int>(v.length()));
                return *this;
            }

            void append(const char *data, int len) { buffer_.append(data, len); }

            Buffer &buffer() { return this->buffer_; }

            template<typename T>
            void formatInterger(T v);

        };

        template<typename T>
        int convert(char *buf, T value);

        template<typename T>
        int convertHEX(char buf[], T value);
    }




#endif
