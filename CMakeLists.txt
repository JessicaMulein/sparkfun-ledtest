cmake_minimum_required(VERSION 3.16)
project(ledtest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE True)
find_library(LIBI2CDRIVER i2cdriver REQUIRED)

add_executable(ledtest src/main.cpp)
set(SOURCE_FILES
        main.cpp
        i2cDriverHelper.cpp
        i2cDriverHelper.h
        sparkx_alpha_led_display.cpp
        sparkx_alpha_led_display.h
)
list(TRANSFORM SOURCE_FILES PREPEND "src/")

add_subdirectory(src/arduino)

if(NOT USE_LINUX_I2C)
    add_library(i2cdriver STATIC EXCLUDE_FROM_ALL src/i2cdriver/c/common/i2cdriver.c)
    target_sources(i2cdriver PUBLIC src/i2cdriver/c/common/i2cdriver.c src/i2cdriver/c/common/i2cdriver.h)
    find_library(i2cdriver i2cdriver REQUIRED)

    target_link_directories(ledtest PUBLIC src/i2cdriver/c/common)
    target_include_directories(ledtest SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/i2cdriver/c/common)
    target_link_libraries(ledtest PUBLIC i2cdriver sparkx_arduino)
    add_dependencies(ledtest sparkx_arduino)

    add_executable(i2ccl src/i2cdriver/c/linux/i2c.c)
    target_link_directories(i2ccl PUBLIC src/i2cdriver/c/common)
    target_include_directories(i2ccl SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/i2cdriver/c/common)
    target_link_libraries(i2ccl PUBLIC i2cdriver)
endif()