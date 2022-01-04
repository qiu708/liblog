set(SOURCE_FILES
        AsyncLogging.cpp
        FileUtil.h
        LogStream.h
        Timestamp.cpp
        AsyncLogging.h
        LogFile.cpp
        Logging.cpp
        Timestamp.h
        LogFile.h
        Logging.h
        FileUtil.cpp
        LogStream.cpp
        StringPiece.h
        noncopyable.h
        LogConfig.cpp
        AsyncLogPool.h
        Log.h
        )

add_library(liblog STATIC ${SOURCE_FILES})
target_link_libraries(liblog pthread rt)

install(TARGETS liblog DESTINATION lib)

file(GLOB HEADERS "*.h")


#message(STATUS "glob headers ${HEADERS}|headers ${HEADERS2}")
install(FILES ${HEADERS} DESTINATION include/liblog)
add_subdirectory(test)

