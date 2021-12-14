//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_NONCOPYABLE_H
#define LIBLOG_NONCOPYABLE_H

    class noncopyable {
    public:
        noncopyable()=default;
        ~noncopyable()=default;
    private:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;
    };

#endif
