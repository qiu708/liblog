//
// Created by qiu on 2021/12/11.
//

#ifndef LIBLOG_FILEUTIL_H
#define LIBLOG_FILEUTIL_H

#include"noncopyable.h"
#include"StringPiece.h"
/*
 * 定义了一个管理文件的类AppendFile
 * 使用append方法来向文件写入信息(设置了内存缓冲，并不是直接写文件)*/
namespace liblog::FileUtil{

    class ReadSmallFile:noncopyable
    {
        static const int kBufferSize=64 * 1024;
    private:
        int fd_;
        int err_;
        char buf_[kBufferSize]{};
    public:
        explicit ReadSmallFile(StringArg filename);
        ~ReadSmallFile();
        template<typename STR>
        int readToString(int maxSize,
                         STR *content, int64_t *fileSize,
                         int64_t *modifyTime,
                         int64_t *createTime);

        int readToBuffer(int *size);

    };
    template<typename STR>
    int ReadFile(StringArg filename,int maxSize,STR* content,int64_t fileSize=NULL,int64_t modifyTime=NULL,int64_t createTime=NULL)
    {
        ReadSmallFile file(filename);
        return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
    }
    class AppendFile:noncopyable
    {
    public:
        AppendFile(StringArg filename);

        ~AppendFile();

        void append(const char *logline, const size_t len);

        void flush();

        size_t writtenBytes() const
        { return writtenBytes_; }

    private:
        size_t write(const char *logline, size_t len);

        FILE *fp_;
        char buffer_[64 * 1024];
        size_t writtenBytes_;
    };
}





#endif //Q_RPC_FILEUTIL_H
