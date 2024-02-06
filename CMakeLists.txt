cmake_minimum_required(VERSION 3.10)
project(GPSParser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(GPSParser main.cpp NMEAParser.cpp)

# If you have additional libraries to link against, use the target_link_libraries command
# target_link_libraries(GPSParser library_name)
