cmake_minimum_required(VERSION 3.8)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("Debug Build")
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    message("Release Build")
endif()

option(ENABLE_TESTS "Enable tests" OFF)
if (${ENABLE_TESTS})
    enable_testing()
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRAPIDJSON_HAS_STDSTRING=1")
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /W4 /w14640 /w14242 /w14254 /w14263 /w14265 /w14287 \
        /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14826 /w14905 /w14906 /w14928")
else()
    if (APPLE)
    elseif(LINUX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches \
            -Wlogical-op -Wuseless-cast")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -pedantic-errors -std=c++17 \
        -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align \
        -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion \
        -Wnull-dereference -Wdouble-promotion -Wformat=2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror -O3 -ggdb3")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE -DNDEBUG")

project(BraceExp)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(SOURCES Parser.cpp Utils/Utils.cpp)
set(INCLUDES Parser.h Utils/Utils.h)

add_subdirectory(src)
if (${ENABLE_TESTS})
    add_subdirectory(tests)
endif()
if (WIN32)
set_property(TARGET braceexp unit_tests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/run")
endif()
