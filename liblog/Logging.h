//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_LOGGING_H
#define LIBLOG_LOGGING_H



#include<string.h>
#include"LogStream.h"
#include"Timestamp.h"
/*
 * 全局变量g_loglevel 定义日志等级
 * 全局函数g_output与g_flush定义如何输出buffer与刷新buffer
 * SourceFile 输出log日志的代码的文件名
 * logger类使用内含类impl将信息写入buffer
 * logger类析构时会调用g_output输出
 * 要更改输出位置仅需使用setOutput修改g_output
 * 定义了宏LOG_DEBUG等来便捷调用logger，每次使用宏都会创建临时对象logger并析构
 * 创建与析构一次logger会输出一条日志
 * */
namespace liblog{

    class Logger {
    public:
        enum LogLevel
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
        };
        class SourceFile
        {
        public:
            template<int N>
            inline SourceFile(const char(&arr)[N])
                    :data_(arr), size_(N - 1)
            {
                const char *slash = strrchr(data_, '/');
                if (slash)
                {
                    data_ = slash + 1;
                    size_ -= static_cast<int>(data_ - arr);
                }
            }
            explicit SourceFile(const char *filename)
                    : data_(filename)
            {
                const char *slash = strrchr(filename, '/');
                if (slash)
                {
                    data_ = slash + 1;
                }
                size_ = static_cast<int >(strlen(data_));
            }
            const char *data_;
            int size_;
        };
        Logger(SourceFile file, int line, const char* name="Default");
        Logger(SourceFile file, int line, LogLevel level,const char* name="Default");
        Logger(SourceFile file, int line, const char *func, LogLevel level,const char* name="Default");
        Logger(SourceFile file, int line, bool toAbort,const char* name="Default");
        ~Logger();
        LogStream &stream()
        { return impl_.stream_; }
        class Impl
        {
            using loglevel=Logger::LogLevel;

        public:
            Impl(LogLevel level, int old_errno, const SourceFile &file, int line,const std::string& name="Default");

            void formatTime();

            void finish();

            Timestamp time_;
            LogStream stream_;
            LogLevel level_;
            int line_;
            SourceFile basename_;
            std::string name_;
        };
    private:
        int file{};
        LogLevel level;
        Impl impl_;

    };



//构造了一个logger对象申请了一个buffer后释放写入文件
//
//#define LOG_TRACE if(liblog::LogConfig::Map()["Default"].level<=liblog::Logger::TRACE) \
//    liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::TRACE,"Default").stream()
//
//#define LOG_TRACE_BY(name) if(liblog::LogConfig::Map()[name].level<=liblog::Logger::TRACE) \
//    liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::TRACE,name).stream()
//
//#define LOG_DEBUG if(liblog::LogConfig::Map()["Default"].level<=liblog::Logger::DEBUG)\
//     liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::DEBUG,"Default").stream()
//
//#define LOG_DEBUG_BY(name) if(liblog::LogConfig::Map()[name].level<=liblog::Logger::DEBUG)\
//     liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::DEBUG,name).stream()
//
//#define LOG_INFO if(liblog::LogConfig::Map()["Default"].level<=liblog::Logger::INFO)  \
//         liblog::Logger(__FILE__,__LINE__,"Default").stream()
//
//#define LOG_INFO_BY(name) if(liblog::LogConfig::Map()[name].level<=liblog::Logger::INFO)  \
//         liblog::Logger(__FILE__,__LINE__,name).stream()
//
//#define LOG_WARN liblog::Logger(__FILE__,__LINE__,liblog::Logger::WARN,"Default").stream()
//
//#define LOG_WARN_BY(name) liblog::Logger(__FILE__,__LINE__,liblog::Logger::WARN,name).stream()
//
//#define LOG_ERROR liblog::Logger(__FILE__,__LINE__,liblog::Logger::ERROR,"Default").stream()
//
//#define LOG_ERROR_BY(name) liblog::Logger(__FILE__,__LINE__,liblog::Logger::ERROR,name).stream()
//
//#define LOG_FATAL liblog::Logger(__FILE__,__LINE__,liblog::Logger::FATAL,"Default").stream()
//
//#define LOG_FATAL_BY(name) liblog::Logger(__FILE__,__LINE__,liblog::Logger::FATAL,name).stream()
//
//#define LOG_SYSERR liblog::Logger(__FILE__,__LINE__, false,"Default").stream()
//
//#define LOG_SYSFATAL liblog::Logger(__FILE__,__LINE__,true,"Default").stream()
//
//#define LOG_SYSERR_BY(name) liblog::Logger(__FILE__,__LINE__, false,name).stream()
//
//#define LOG_SYSFATAL_BY(name) liblog::Logger(__FILE__,__LINE__,true,name).stream()

}



#endif
