//
// Created by qiu on 2021/11/30.
//

#include "Logging.h"
#include <assert.h>
#include <syscall.h>
#include <zconf.h>
#include <unistd.h>
#include"AsyncLogging.h"
#include<liblog/LogConfig.h>
#include <liblog/AsyncLogPool.h>
using namespace liblog;

const char* LogLevelName[6] =
        {
                "\033[32mTRACE ",
                "\033[34mDEBUG ",
                "\033[36mINFO  ",
                "\033[33mWARN  ",
                "\033[31mERROR ",
                "\033[31mFATAL ",
        };
    Logger::LogLevel initLogLevel()
    {
        return Logger::TRACE;
    }




    class T {
    public:
        T(const char *str, unsigned len) :
                str_(str), len_(len) {
            assert(strlen(str) == len_);
        }

        const char *str_;
        const unsigned len_;
    };

    inline LogStream &operator<<(LogStream &s, T v) {
        s.append(v.str_, v.len_);
        return s;
    }

    inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v) {
        s.append(v.data_, v.size_);
        return s;
    }

    Logger::Impl::Impl(Logger::LogLevel level, int old_errno, const Logger::SourceFile &file, int line,const std::string& name):time_(Timestamp::now()),level_(level),
                                                                                                        basename_(file),line_(line),
                                                                                                        stream_(),name_(name){
        stream_<<"\033[0m";
        formatTime();
        std::string s = std::to_string((::syscall(SYS_gettid)));
        s += " ";
        stream_ << T(s.c_str(), s.size());
        stream_<<LogLevelName[level];
//        stream_<<"old errno"<<old_errno<<"errno"<<errno;
        if (old_errno != 0)
        {

            stream_ << " (errno=" << old_errno << ") , ErrorMessage:"<< strerror(old_errno)<<" ";
        }
    }
    void Logger::Impl::finish()
    {
        stream_ << " - "<< basename_.data_<< ':'<< line_ << "\033[0m"<<'\n';
    }

    void Logger::Impl::formatTime() {
        this->stream_<<Timestamp::now().toformattedString()<<" ";
    }

    Logger::Logger(SourceFile file, int line,const char*  name): impl_(INFO, 0, file, line,name)
    {
    }

    //Logger(SourceFile file, int line, LogLevel level, const char *func);

    Logger::Logger (SourceFile  file, int line, const char *func,LogLevel level,const char* name)
            : impl_(level, 0, file, line,name)
    {
        impl_.stream_ << func << ' ';
    }

    Logger::Logger(SourceFile file, int line, LogLevel level,const char* name)
            : impl_(level, errno, file, line,name)
    {
    };

    Logger::Logger(SourceFile file, int line, bool toAbort,const char* name)
            : impl_(toAbort ? FATAL : ERROR, errno, file, line,name)
    {}

    Logger::~Logger()
    {
        impl_.finish();
        if(LogConfig::Map()[impl_.name_].toFile)
        {
            if(LogConfig::Map()[impl_.name_].async)
            {
                AsyncLogPool::getInstance(impl_.name_).append(this->impl_.stream_.buffer().data(), this->impl_.stream_.buffer().bufferlength());
            }
            if(impl_.level_==LogLevel::FATAL)
            {
                fflush(stdout);
                abort();
            }
        }else
        {
            fwrite(this->impl_.stream_.buffer().data(), 1, this->impl_.stream_.buffer().bufferlength(), stdout);
            if(impl_.level_==LogLevel::FATAL)
            {
                fflush(stdout);
                abort();
            }
        }

    }






