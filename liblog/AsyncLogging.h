//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_ASYNCLOGGING_H
#define LIBLOG_ASYNCLOGGING_H
#include"Timestamp.h"
#include"noncopyable.h"
#include"LogFile.h"
#include"LogStream.h"
#include<vector>
#include<atomic>
#include<thread>
#include<mutex>
#include<condition_variable>
#include <unordered_map>

/*
 * 使用了两个vector AB保存缓冲区
 *
 * 类私有成员: Buffer_vector buffers_(A)
 *           Buffer currentbuf_ nextbuf
 * 线程局部变量: Buffer_vector buffertowrite
 *             Buffer back_1 back_2
 * 使用异步日志时使用append先向currentbuf_写（不唤醒读写线程）
 * 若写满则将currentbuf_放入buffers_ 将nextbuf_作为currentbuf_ （唤醒读写线程）
 * 若nextbuf为空则创建新的buf
 *
 * 读写线程被唤醒后将当前currentbuf_推入buffers_
 * 交换buffers_与buffertowrite(buffertowrite.size()==0)
 * 并将back_1与back_2作为新的currentbuf_和nextbuf_
 * 开始写文件
 * 写完后若buffertowrite大小大于2则将buffertowrite大小置为2
 * 将buffertowrite内的buffer交给back_1与back_2
 * 循环
 * */
namespace liblog{
    struct LogSetting;
    class AsyncLogging:noncopyable{
    public:
        AsyncLogging(const std::string& basename,size_t rollSize,int flushInterval=1);
        ~AsyncLogging()
        {
            if(isrunning)
            {
                stop();
            }
        }

    public:
        void append(const char* logline,int len);
        void start();
        void stop()
        {
            isrunning=false;
            threadptr->join();
        }

    private:
        void threadfunc();

    public:
        void setBasename(const std::string &basename);

        void setRollSize(size_t rollSize);

        void setFlushInterval(int flushInterval);

    private:
        std::string basename_;
        size_t rollSize_;
        int flushInterval_;
        using Buffer=FixedBuffer<16000000>;
        using Buffer_ptr=std::unique_ptr<Buffer>;
        using Buffer_vector=std::vector<Buffer_ptr>;
        Buffer_ptr currnetbuf_;
        Buffer_ptr nextbuf_;
        Buffer_vector buffers_;
        std::atomic_bool isrunning;
        std::unique_ptr<std::thread> threadptr;
        std::condition_variable cv_;
        std::mutex mutex_;
        time_t logfiletimeout_;
        Timestamp lastChecklog_;
        time_t logCheckInterval;
    public:
        void setLogCheckInterval(time_t logCheckInterval);

    public:
        void setLogfiletimeout(time_t logfiletimeout);
    };

}



#endif
