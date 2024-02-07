cmake_minimum_required(VERSION 3.10)
project(NMEAParser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DEBUG_BUILD "Enable debug build" OFF)
option(LOG_RAW "Log raw NMEA data to a file raw.txt" OFF)


if(DEBUG_BUILD)
    add_definitions(-DDEBUG_BUILD)
endif()

if(LOG_RAW)
    add_definitions(-DLOG_RAW)
endif()

include_directories(src)

# Add the executable
add_executable(NMEAParser
    src/main.cpp
    src/NMEAParser.cpp
)

# ulg generation for analysis
add_subdirectory(third_party/ulog_cpp)

target_link_libraries(NMEAParser PUBLIC
    ulog_cpp::ulog_cpp
)
