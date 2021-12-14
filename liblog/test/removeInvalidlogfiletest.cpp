//
// Created by qiu on 2021/12/13.
//
#include<thread>
#include<chrono>
#include<liblog/AsyncLogging.h>
#include<liblog/Logging.h>
int main()
{
    liblog::Logger::setOutput(liblog::asyncoutput);
    liblog::AsyncLogging::getinstance().setBasename("/home/qiu/Desktop/invalidlog/log");
    liblog::AsyncLogging::getinstance().setFlushInterval(1);
    liblog::AsyncLogging::getinstance().setRollSize(1);
    liblog::AsyncLogging::getinstance().setLogfiletimeout(10);
    for(int i=0;i<200;i++)
    {
        LOG_DEBUG<<"test--"<<i;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout<<i;
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

