//
// Created by qiu on 2021/12/4.
//

#include "LogFile.h"

#include <memory>
#include "StringPiece.h"
#include "FileUtil.h"
#include<filesystem>
#include"Timestamp.h"
using namespace liblog;
using namespace liblog::FileUtil;
LogFile::LogFile(const std::string &basename, size_t rollSize, bool threadSafe, int flushInterval,
                      int checkEveryN)
        :basename_(basename),
         rollsize_(rollSize),
         mutex_(threadSafe?new std::mutex: nullptr),
         flushInterval_(flushInterval),
         checkEveryN_(checkEveryN),
         count_(0),
         startOfPeriod_(0),
         lastFlush_(0),
         lastRoll_(0),
         basepath_(getLogFilePath(basename)),
         elapse_(std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count())
{
    rollFile();
}

bool LogFile::rollFile() {
    time_t now=0;
    std::string filename= getLogFileName(basename_,&now);
    time_t start=now/kRollPerSeconds_*kRollPerSeconds_;
    if(now>lastRoll_)
    {
        lastRoll_=now;
        lastFlush_=now;
        startOfPeriod_=start;
//        FileUtil::AppendFile file(filename)
        file_ = std::make_unique<FileUtil::AppendFile>(filename);
        return true;
    }
    return false;
}

std::string LogFile::getLogFileName(const std::string &basename, time_t *now) {
    std::string filename;
    filename.reserve(basename.size()+64);
    filename=basename;
    char timebuf[32];
    struct tm tm;
    *now=time(nullptr);
    gmtime_r(now,&tm);
    strftime(timebuf,sizeof timebuf,".%Y%m%d-%H%M%S",&tm);
    filename+=timebuf;
//    filename+=ProcessInfo::hostname();
    filename+=".log";
    return filename;
}

void LogFile::append(const char *logline, int len) {
    if(mutex_)
    {
        std::lock_guard<std::mutex> guard(*mutex_);
        append_unlocked(logline,len);
    }else
    {
        append_unlocked(logline,len);
    }
}

void LogFile::append_unlocked(const char *logline, int len) {
    file_->append(logline,len);
    if(file_->writtenBytes()>rollsize_) {
        std::cout<<"roll due to filesize,filesize:"<<file_->writtenBytes()<<std::endl;
        rollFile();
    }
    else
    {
        ++count_;
        if(count_>=checkEveryN_)
        {
            count_=0;
            time_t now=::time(NULL);
            time_t thisPeriod_=now/kRollPerSeconds_*kRollPerSeconds_;
            std::cout<<"thisperiod"<<thisPeriod_<<" now:"<<now<<" lastflush:"<<lastFlush_<<std::endl;
            if(thisPeriod_!=startOfPeriod_)
            {
                std::cout<<"roll due to period"<<std::endl;
                rollFile();
            }
            else if(now-lastFlush_ >= flushInterval_)
            {
                std::cout<<"flush due to flushinterval"<<std::endl;
                lastFlush_=now;
                file_->flush();
            }
        }
    }
}

void LogFile::flush() {
    if(mutex_)
    {
        std::lock_guard<std::mutex>  g(*mutex_);
        file_->flush();
    }
    else
        file_->flush();
}

std::string LogFile::getLogFilePath(std::string name) {
    std::string path=name;
    while(!path.empty()&&path.back()!='/')
    {
        path.pop_back();
    }
    if(path.empty()) return "./";
    return path;

}

void LogFile::removeInvalid(time_t durationSeconds) {
    if(durationSeconds==0) return;
    for(auto&p:std::filesystem::directory_iterator(basepath_)) {
        if (p.is_directory()) continue;
        time_t systemTime =
                std::chrono::duration_cast<std::chrono::seconds>(p.last_write_time().time_since_epoch()).count() -
                elapse_;
        Timestamp lastwrite(systemTime * 1000000);
        if (Timestamp::now().microSecondsSinceEpoch() - lastwrite.microSecondsSinceEpoch() >
            durationSeconds * 1000000) {
            std::filesystem::remove(p.path());
        }
    }
}

