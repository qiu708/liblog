//
// Created by qiu on 2021/12/13.
//
#include<liblog/LogFile.h>
int main()
{
    liblog::LogFile f("/home/qiu/Desktop/log/log",600);
    f.removeInvalid(10*24*3600);
}

