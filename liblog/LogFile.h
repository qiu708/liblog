//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_LOGFILE_H
#define LIBLOG_LOGFILE_H

#include<iostream>
#include<mutex>
#include<memory>
#include "noncopyable.h"
#include"FileUtil.h"
/*logfile
 * 使用append接口将数据写入文档,自动滚动*/
namespace liblog{
    namespace FileUtil
    {
        class AppendFile;
    }
    class LogFile:noncopyable {
    public:
        /*basename:文件名称，可以带路径，生成文件名${basename}20210120000.log，
         * rollsize:文件滚动大小，超过该大小自动创建下一个文件，如果时间间隔过小文件名重复则会滚动失败继续向当前文件写入
         * threadsafe:是否线程安全
         * checkEveryN:每写入n条日志检查是否需要刷新文件描述符或滚动
         * flushinterval:当最后刷新时间与当前时间间隔多少秒时刷新文件描述符*/
        LogFile(const std::string &basename,size_t rollSize,bool threadSafe=true,int flushInterval=3,int checkEveryN=1024);
        ~LogFile()=default;
        void append(const char* logline,int len);
        void flush();
        bool rollFile();
        void removeInvalid(time_t durationSeconds);
    private:
        void append_unlocked(const char * logline,int len);
        static std::string getLogFileName(const std::string &basename,time_t * now);
        std::string getLogFilePath(std::string name);
        const std::string basename_;
        const std::string basepath_;
        const size_t rollsize_;
        const int flushInterval_;
        const int checkEveryN_;
        int count_;
        std::unique_ptr<std::mutex> mutex_;
        time_t startOfPeriod_;
        time_t lastRoll_;
        time_t lastFlush_;
        std::unique_ptr<FileUtil::AppendFile> file_;
        const static int kRollPerSeconds_=60*60*24;
        long elapse_;


    };

}



#endif
