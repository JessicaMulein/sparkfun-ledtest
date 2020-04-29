cmake_minimum_required(VERSION 3.16)
project(sparkx_arduino CXX C)
set(CMAKE_VERBOSE_MAKEFILE True)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    Print.h
    Print.cpp
    WString.cpp
    WString.h
    stdlib_noniso.h
    stdlib_noniso.c
)
#list(TRANSFORM SOURCE_FILES PREPEND "src/")

add_library(sparkx_arduino STATIC stdlib_noniso.c)