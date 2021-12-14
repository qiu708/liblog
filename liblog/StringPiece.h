//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_STRINGPIECE_H
#define LIBLOG_STRINGPIECE_H

#include<iostream>
#include<string>
#include<string.h>
namespace liblog{
    class StringArg
    {
    public:
        StringArg(const char* s):str_(s){}
        StringArg(std::string &s):str_(s.c_str()){}
        const char* c_str()
        {
            return str_;
        }
    private:
        const char* str_;
    };
    class StringPiece
    {

    public:
        StringPiece() : ptr_(nullptr), length_(0)
        {}

        StringPiece(const char *str) : ptr_(str), length_(static_cast<int >(strlen(ptr_)))
        {}

        StringPiece(const unsigned char *str) :
                ptr_(reinterpret_cast<const char *>(str)),
                length_(static_cast<int >(strlen(ptr_)))
        {}

        StringPiece(const std::string &str) :
                ptr_(str.data()), length_(static_cast<int>(str.size()))
        {}

        StringPiece(const char *offset, int len) : ptr_(offset), length_(len)
        {}

        [[nodiscard]] const char *data() const
        { return ptr_; }

        [[nodiscard]] int size() const
        { return length_; }

        bool empty() const
        { return length_ == 0; }

        const char *const begin()
        { return ptr_; }

        const char *const end()
        { return ptr_ + length_; }

        void clear()
        {
            ptr_ = nullptr;
            length_ = 0;
        }

        void set(const char *buffer, int len)
        {
            ptr_ = buffer;
            length_ = len;
        }

        void set(const void *buffer, int len)
        {
            ptr_ = reinterpret_cast<const char *>(buffer);
            length_ = len;
        }

        char operator[](int i) const
        { return ptr_[i]; }

        void remove_prefix(int n)
        {
            ptr_ += n;
            length_ -= n;
        }

        void remove_suffix(int n)
        {
            length_ -= n;
        }

        bool operator==(const StringPiece &x) const
        {
            return ((length_ == x.length_) && memcmp(ptr_, x.ptr_, length_));
        }

        bool operator!=(const StringPiece &x) const
        { return !(*this == x); }


#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)  \
        bool operator cmp (const StringPiece& x)const{  \
        int r=memcmp(ptr_,x.ptr_,length_<x.length_? length_:x.length_);\
        return ((r auxcmp 0)|| ((r==0 ) && (length_ cmp x.length_)));  \
    }


        STRINGPIECE_BINARY_PREDICATE(<, <);

        STRINGPIECE_BINARY_PREDICATE(<=, <);

        STRINGPIECE_BINARY_PREDICATE(>=, >);

        STRINGPIECE_BINARY_PREDICATE(>, >);
#undef STRINGPIECE_BINARY_PREDICATE

        int compare(const StringPiece &x) const
        {
            int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
            if (r == 0)
            {
                if (length_ < x.length_)
                    r = -1;
                else if (length_ > x.length_)
                    r = +1;
            }
            return r;
        }

        std::string as_string() const
        {
            return std::string(data(),size());
        }

        void CopyToString(std::string* target)const
        {
            target->assign(ptr_,length_);
        }

        bool starts_with(const StringPiece& x)const
        {
            return ((length_>=x.length_)&&(memcmp(ptr_,x.ptr_,x.length_)==0));
        }

    private:
        const char *ptr_;
        int length_;

    };


}


#endif
