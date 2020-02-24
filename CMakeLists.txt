cmake_minimum_required(VERSION 3.10)

#set the project name
project(JobPM VERSION 0.5)

# get the version value
configure_file(JOB_holder.h.in JOB_holder.h)


#add the exexutable
add_executable(JPM main.cpp)

# setup c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
