cmake_minimum_required(VERSION 3.10)

#set the project name
project(JobPM VERSION 0.5)

# get the version value
configure_file(JOB_holder.h.in JOB_holder.h)
configure_file(TimeUtility.h.in TimeUtility.h)

# setup c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the exexutable - actually, "JPM" is the target
add_executable(JPM main.cpp JOB_holder.cpp TimeUtility.cpp)

target_include_directories(JPM PUBLIC $(PROJECT_BINARY_DIR))
