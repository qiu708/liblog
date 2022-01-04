//
// Created by qiu on 2022/1/4.
//

#ifndef LIBLOG_LOG_H
#define LIBLOG_LOG_H
#include <liblog/LogConfig.h>
#include <liblog/Logging.h>
#include <liblog/AsyncLogPool.h>


#define LOG_TRACE if(liblog::LogConfig::Map()["Default"].level<=liblog::Logger::TRACE) \
    liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::TRACE,"Default").stream()

#define LOG_TRACE_BY(name) if(liblog::LogConfig::Map()[name].level<=liblog::Logger::TRACE) \
    liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::TRACE,name).stream()

#define LOG_DEBUG if(liblog::LogConfig::Map()["Default"].level<=liblog::Logger::DEBUG)\
     liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::DEBUG,"Default").stream()

#define LOG_DEBUG_BY(name) if(liblog::LogConfig::Map()[name].level<=liblog::Logger::DEBUG)\
     liblog::Logger(__FILE__,__LINE__,__func__,liblog::Logger::DEBUG,name).stream()

#define LOG_INFO if(liblog::LogConfig::Map()["Default"].level<=liblog::Logger::INFO)  \
         liblog::Logger(__FILE__,__LINE__,"Default").stream()

#define LOG_INFO_BY(name) if(liblog::LogConfig::Map()[name].level<=liblog::Logger::INFO)  \
         liblog::Logger(__FILE__,__LINE__,name).stream()

#define LOG_WARN liblog::Logger(__FILE__,__LINE__,liblog::Logger::WARN,"Default").stream()

#define LOG_WARN_BY(name) liblog::Logger(__FILE__,__LINE__,liblog::Logger::WARN,name).stream()

#define LOG_ERROR liblog::Logger(__FILE__,__LINE__,liblog::Logger::ERROR,"Default").stream()

#define LOG_ERROR_BY(name) liblog::Logger(__FILE__,__LINE__,liblog::Logger::ERROR,name).stream()

#define LOG_FATAL liblog::Logger(__FILE__,__LINE__,liblog::Logger::FATAL,"Default").stream()

#define LOG_FATAL_BY(name) liblog::Logger(__FILE__,__LINE__,liblog::Logger::FATAL,name).stream()

#define LOG_SYSERR liblog::Logger(__FILE__,__LINE__, false,"Default").stream()

#define LOG_SYSFATAL liblog::Logger(__FILE__,__LINE__,true,"Default").stream()

#define LOG_SYSERR_BY(name) liblog::Logger(__FILE__,__LINE__, false,name).stream()

#define LOG_SYSFATAL_BY(name) liblog::Logger(__FILE__,__LINE__,true,name).stream()

#endif //LIBLOG_LOG_H
