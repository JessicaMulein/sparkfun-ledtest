cmake_minimum_required(VERSION 3.16)
project(sparkx_arduino CXX C)
set(CMAKE_VERBOSE_MAKEFILE True)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    Print.h
    Print.cpp
    Printable.h
)
#list(TRANSFORM SOURCE_FILES PREPEND "src/")

add_library(sparkx_arduino STATIC ${SOURCE_FILES})