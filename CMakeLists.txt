cmake_minimum_required(VERSION 3.3)
project(LinkedHashMap)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(LinkedHashMap ${SOURCE_FILES})