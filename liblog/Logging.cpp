//
// Created by qiu on 2021/11/30.
//

#include "Logging.h"
#include <assert.h>
#include <syscall.h>
#include <zconf.h>
#include <unistd.h>
#include"AsyncLogging.h"
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
    Logger::LogLevel liblog::g_loglevel=initLogLevel();
    void defaultOutput(const char *msg, int len)//默认是标准输出．
    {
        size_t n = fwrite(msg, 1, len, stdout);
    }

    void defaultFlush()
    {
        fflush(stdout);
    }
    void liblog::asyncoutput(const char *msg, int len)
    {
//        std::cout<<"---asynclog---"<<msg<<std::endl;
        AsyncLogging::getinstance().append(msg,len);
    }
    void liblog::asyncflush()
    {

    }


    Logger::FlushFunc g_flush=defaultFlush;
    Logger::OutputFunc  g_output=defaultOutput;

//    Logger::FlushFunc g_flush=asyncflush;
//    Logger::OutputFunc  g_output=asyncoutput;

    void Logger::setLogLevel(Logger::LogLevel level) {
        g_loglevel=level;
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

    Logger::Impl::Impl(Logger::LogLevel level, int old_errno, const Logger::SourceFile &file, int line):time_(Timestamp::now()),level_(level),
                                                                                                        basename_(file),line_(line),
                                                                                                        stream_(){
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

    Logger::Logger(SourceFile file, int line): impl_(INFO, 0, file, line)
    {
    }

    //Logger(SourceFile file, int line, LogLevel level, const char *func);

    Logger::Logger (SourceFile  file, int line, LogLevel level, const char *func)
            : impl_(level, 0, file, line)
    {
        impl_.stream_ << func << ' ';
    }

    Logger::Logger(SourceFile file, int line, LogLevel level)
            : impl_(level, errno, file, line)
    {
    };

    Logger::Logger(SourceFile file, int line, bool toAbort)
            : impl_(toAbort ? FATAL : ERROR, errno, file, line)
    {}

    Logger::~Logger()
    {
        impl_.finish();
        g_output(this->impl_.stream_.buffer().data(), this->impl_.stream_.buffer().bufferlength());
        if(impl_.level_==LogLevel::FATAL)
        {
            g_flush();
            abort();
        }
    }
    void Logger::setOutput(OutputFunc out)
    {
        g_output = out;
    }

    void Logger::setFlush(FlushFunc flush)
    {
        g_flush = flush;
    }





