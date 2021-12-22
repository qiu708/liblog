//
// Created by qiu on 2021/12/21.
//

#ifndef LIBLOG_LOGCONFIG_H
#define LIBLOG_LOGCONFIG_H
#include<libconfig/Config.h>
#include <string>
#include <liblog/Logging.h>
#include <map>
namespace liblog{
    class Logger;
    struct LogSetting
    {
        Logger::LogLevel level;
        bool toFile;
        bool async;
        std::string logFilePath;
        std::string logFilePrefix;
    };
    class LogConfig:noncopyable
    {
    public:
        static std::unordered_map<std::string,LogSetting>& Map()
        {
            static LogConfig instance;
            return instance.configMap_;
        }
    private:
        LogConfig()
        {
            for(auto& name: GET_CONFIG("Log",std::vector<std::string>(1,"default")))
            {
                configMap_[name].level= getConfigLogLevel(name);
                configMap_[name].toFile= GET_CONFIG("Log."+name+".ToFile",false);
                configMap_[name].async= GET_CONFIG("Log."+name+".Async",true);
                configMap_[name].logFilePath= GET_CONFIG("Log."+name+".LogFilePath",std::string("./"));
                if(configMap_[name].logFilePath.back()!='/') configMap_[name].logFilePath.push_back('/');
                configMap_[name].logFilePrefix= GET_CONFIG("Log."+name+".LogFilePrefix",name);
                std::cout<<"name:"<<name<<"\n"
                        <<"configMap_[name].level:"<<configMap_[name].level<<"\n"
                        <<"configMap_[name].toFile:"<<configMap_[name].toFile<<"\n"
                        <<"configMap_[name].async:"<<configMap_[name].async<<"\n"
                        <<"configMap_[name].logFilePath:"<<configMap_[name].logFilePath<<"\n"
                        <<"configMap_[name].logFilePrefix:"<<configMap_[name].logFilePrefix<<"\n";
            }
        }
        Logger::LogLevel getConfigLogLevel(std::string name)
        {
            auto level= GET_CONFIG("Log."+name+".LogLevel",std::string("TRACE"));
            if(level=="TRACE")
            { return Logger::TRACE; }
            if(level=="DEBUG")
            { return Logger::DEBUG; }
            if(level=="INFO")
            { return Logger::INFO; }
            if(level=="WARN")
            { return Logger::WARN; }
            if(level=="FATAL")
            { return Logger::FATAL; }
            if(level=="ERROR")
            { return Logger::ERROR; }
            return Logger::WARN;
        }

        std::unordered_map<std::string,LogSetting> configMap_;

    };
}


#endif //LIBLOG_LOGCONFIG_H
