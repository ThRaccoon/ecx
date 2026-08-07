cmake_minimum_required(VERSION 3.21)
project(ECX LANGUAGES CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "ECX only supports 64-bit builds")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ecx)
add_subdirectory(tests)
add_subdirectory(examples)
