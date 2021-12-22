//
// Created by qiu on 2021/12/22.
//

#ifndef LIBLOG_ASYNCLOGPOOL_H
#define LIBLOG_ASYNCLOGPOOL_H

#include<liblog/AsyncLogging.h>
#include <liblog/LogConfig.h>

namespace liblog {
    class AsyncLogPool:noncopyable {

    public:
        static AsyncLogging& getInstance(const std::string& name)
        {
            static AsyncLogPool instance(LogConfig::Map());
            if(instance.logMap_.find(name)!=instance.logMap_.end())
                return *instance.logMap_[name];
        }
    private:
        AsyncLogPool(std::unordered_map<std::string,LogSetting>& configMap){
            for(auto& p:configMap)
            {
                if(p.second.toFile && p.second.async)
                {
                    logMap_.emplace(p.first,new AsyncLogging(p.second.logFilePath+p.second.logFilePrefix,6400));
                }
            }
        }
        std::unordered_map<std::string,std::unique_ptr<AsyncLogging> > logMap_;
    };
}


#endif //LIBLOG_ASYNCLOGPOOL_H
