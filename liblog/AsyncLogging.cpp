//
// Created by qiu on 2021/12/4.
//

#include "AsyncLogging.h"
#include<assert.h>
#include"Timestamp.h"
#include <memory>
#include<unistd.h>
using namespace liblog;
AsyncLogging::AsyncLogging(const std::string &basename, size_t rollSize, int flushInterval)
        :basename_(basename),
         rollSize_(rollSize),
         flushInterval_(flushInterval),
         currnetbuf_(new Buffer),
         nextbuf_(new Buffer),
         isrunning(false),
         logfiletimeout_(0),
         lastChecklog_(Timestamp::now()),
         logCheckInterval(0)
{
    currnetbuf_->bzero();
    nextbuf_->bzero();
    buffers_.reserve(16);
}

void AsyncLogging::threadfunc() {
    // FIXME: if is exited before entering the thread
    assert(isrunning==true);
    std::unique_ptr<LogFile> output(new LogFile(basename_,rollSize_,false));
    Buffer_ptr back_1(new Buffer);
    Buffer_ptr back_2(new Buffer);
    back_1->bzero();
    back_2->bzero();
    Buffer_vector buffertowrite;
    buffertowrite.reserve(16);
    std::chrono::seconds interval(flushInterval_);
    while(isrunning)
    {
        assert(buffertowrite.empty());
        {
            std::unique_lock<std::mutex> guard(mutex_);
            if(buffers_.empty())
            {
                cv_.wait_for(guard,interval);
            }
            buffers_.push_back(std::move(currnetbuf_));
            currnetbuf_=std::move(back_1);
            buffertowrite.swap(buffers_);
            if(nextbuf_)
            {
                nextbuf_=std::move(back_2);
            }
        }
        if(buffertowrite.size()>25)
        {
            //pass
            char buf_[256];
            snprintf(buf_, sizeof(buf_), "Drop log messages at %s,%zd larger buffers\n",
                     Timestamp::now().toformattedString().c_str(), buffertowrite.size() - 2);
            fputs(buf_, stderr);
            output->append(buf_, static_cast<int>(strlen(buf_)));
            buffertowrite.erase(buffertowrite.begin() + 2, buffertowrite.end());
        }
        for(auto& buf:buffertowrite)
        {
//            std::cout<<"append data to file"<<buf->data()<<std::endl;
            output->append(buf->data(),buf->bufferlength());
        }
        if(logCheckInterval!=0&&Timestamp::now().microSecondsSinceEpoch()-lastChecklog_.microSecondsSinceEpoch()>=logCheckInterval*1000000)
        {
            output->removeInvalid(logfiletimeout_);
            lastChecklog_=Timestamp::now();
        }
        if(buffertowrite.size()>2)
        {
            buffertowrite.resize(2);
        }
        if(!back_1)
        {
            assert(!buffertowrite.empty());
            back_1=std::move(buffertowrite.back());
            buffertowrite.pop_back();
            back_1->reset();
        }
        if(!back_2)
        {
            assert(!buffertowrite.empty());
            back_2=std::move(buffertowrite.back());
            buffertowrite.pop_back();
            back_2->reset();
        }
        buffertowrite.clear();
        output->flush();

    }
    output->flush();

}

void AsyncLogging::start() {
    isrunning=true;
    assert(isrunning==true);
    threadptr = std::make_unique<std::thread>(&AsyncLogging::threadfunc,this);
}

void AsyncLogging::append(const char *logline, int len) {
    if(!isrunning) start();
    std::lock_guard<std::mutex> guard(mutex_);

    if(len>currnetbuf_->avail())
    {
        currnetbuf_->append(logline,len);
    }
    {
        buffers_.push_back(std::move(currnetbuf_));
        if(nextbuf_)
        {
            currnetbuf_=std::move(nextbuf_);
        }else
        {
            currnetbuf_ = std::make_unique<Buffer>();
        }
        currnetbuf_->append(logline,len);
        cv_.notify_one();
    }



}

void AsyncLogging::setBasename(const std::string &basename) {
    basename_ = basename;
}

void AsyncLogging::setRollSize(size_t rollSize) {
    rollSize_ = rollSize;
}

void AsyncLogging::setFlushInterval(int flushInterval) {
    flushInterval_ = flushInterval;
}

void AsyncLogging::setLogfiletimeout(time_t logfiletimeout) {
    logfiletimeout_ = logfiletimeout;
}

void AsyncLogging::setLogCheckInterval(time_t logCheckInterval) {
    AsyncLogging::logCheckInterval = logCheckInterval;
}
