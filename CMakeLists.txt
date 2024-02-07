cmake_minimum_required(VERSION 3.10)
project(NMEAParser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DEBUG_BUILD "Enable debug build" OFF)

if(DEBUG_BUILD)
    add_definitions(-DDEBUG_BUILD)
endif()

include_directories(src)

# Add the executable
add_executable(NMEAParser
    src/main.cpp
    src/NMEAParser.cpp
)

# If you have additional libraries to link against, use the target_link_libraries command
# target_link_libraries(NMEAParser library_name)
