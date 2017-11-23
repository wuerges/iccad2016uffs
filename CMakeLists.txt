cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

project (ICCAD_2016_B)
include_directories(include)
add_subdirectory("ext-lib/rapidcheck")
add_subdirectory("lib")
add_subdirectory("test")
add_subdirectory("app")
# subdirs(app)


