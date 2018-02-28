PROJECT(dayTimer C)
cmake_minimum_required(VERSION 3.9)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

message(STATUS "Current Build Mode: ${CMAKE_BUILD_TYPE}")

INCLUDE(CheckCCompilerFlag)

SET(LIBUCL_VERSION_MAJOR 0)
SET(LIBUCL_VERSION_MINOR 1)
SET(LIBUCL_VERSION_PATCH 0)

CHECK_C_COMPILER_FLAG(-Wall SUPPORT_WALL)
CHECK_C_COMPILER_FLAG(-W SUPPORT_W)
CHECK_C_COMPILER_FLAG(-Wno-unused-parameter SUPPORT_WPARAM)
CHECK_C_COMPILER_FLAG(-Wno-pointer-sign SUPPORT_WPOINTER_SIGN)
CHECK_C_COMPILER_FLAG(-Wstrict-prototypes SUPPORT_WSTRICT_PROTOTYPES)

#SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}'  as none was specified.")

#include_directories(libs/libucl)
#link_directories(libs/libucl)

#find_library(UCL_LIB ucl, /usr/local/include)
add_executable(dayTimer main.c days.c)

add_executable(tests_days tests_days.c days.c)

SET_TARGET_PROPERTIES(dayTimer PROPERTIES COMPILE_FLAGS -g)
#target_link_libraries(magic ucl)
