//
// Created by qiu on 2021/12/12.
//
#include<iostream>
#include<thread>
#include<liblog/Logging.h>
#include<liblog/AsyncLogging.h>
#include<filesystem>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std::chrono_literals;
int main()
{
    std::cout<<liblog::Timestamp::now().toformattedString()<<std::endl;
    LOG_DEBUG<<"test";
    liblog::Logger::setOutput(liblog::asyncoutput);
    liblog::AsyncLogging::getinstance().setBasename("/home/qiu/Desktop/log/log");
    LOG_DEBUG<<"asynclogtest-1";
    std::this_thread::sleep_for(std::chrono::seconds(10));//防止在进入异步线程前就对异步对象析构




}