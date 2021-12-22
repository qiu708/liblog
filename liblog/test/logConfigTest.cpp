//
// Created by qiu on 2021/12/21.
//

#include <liblog/LogConfig.h>
#include <liblog/Logging.h>
#include <liblog/AsyncLogging.h>
#include <liblog/AsyncLogPool.h>
#include <thread>
#include <chrono>
#include<iostream>
#include <string>


int main()
{
    LOG_TRACE_BY("Net")<<"net";
    LOG_TRACE<<"trace";
    LOG_DEBUG_BY("Net")<<"net";
    LOG_DEBUG<<"debug";
    LOG_INFO_BY("Net")<<"net";
    LOG_INFO<<"info";
    LOG_WARN_BY("Net")<<"net";
    LOG_WARN<<"warn";
    LOG_ERROR_BY("net")<<"net";
    LOG_ERROR<<"error";
    LOG_SYSERR_BY("Net")<<"net";
    LOG_SYSERR<<"syserror";
    std::this_thread::sleep_for(std::chrono::seconds(10));

}