cmake_minimum_required (VERSION 3.10.2)
project (ECDLP-Diem)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set (CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined -g -fsanitize-undefined-trap-on-error")
set (CMAKE_CXX_FLAGS_RELEASE "-march=native -O3 -DNDEBUG")

add_executable (trans trans.cpp)
add_executable (recv recv.cpp)

