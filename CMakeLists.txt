cmake_minimum_required(VERSION 3.8)

project(ga3)

##### Use C++14 or 17 when available
include(CheckCXXCompilerFlag)

# Check for standard to use. This is going to depend on compiler and version, because I couldn't get check_cxx_compile_flag to work
message(STATUS ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})

#check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
#if(HAVE_FLAG_STD_CXX17)
#    # Have -std=c++17, use it
#    message(STATUS "GA3 using C++17")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#else()
#    check_cxx_compiler_flag(-std=c++14 HAVE_FLAG_STD_CXX14)
#    if(HAVE_FLAG_STD_CXX14)
#        # Have -std=c++14, use it
        message(STATUS "GA3 using C++14")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#    else()
#        message(FATAL_ERROR "GA3 requires at least C++14")
#    endif()
#endif()


include(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()


##### Options

macro(ga3_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()
    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()

message(STATUS "ga3 CMake Options")
message(STATUS "=======================================================")
ga3_option(BUILD_GA3_TESTS    "Build the test suite"                  OFF)
ga3_option(BUILD_GA3_COVERAGE "Generate test coverage information"    OFF)
ga3_option(BUILD_GA3_EXAMPLES "Build the examples"                    OFF)
message(STATUS "=======================================================")


message(STATUS "ga3 version: " ${CONAN_PACKAGE_VERSION})
add_definitions(-DGA3_NAME=\"${CONAN_PACKAGE_NAME}\")
add_definitions(-DGA3_VERSION=\"${CONAN_PACKAGE_VERSION}\")


include_directories(SYSTEM PRIVATE .)

set(SOURCE_FILES
        ga3/optional.hpp
        ga3/gene.hpp
        ga3/chromosome.hpp
        ga3/chromosome.cpp
        ga3/population.hpp
        ga3/population.cpp
        ga3/ga3.hpp
        ga3/types.hpp
        )

set(THREADPOOL_SOURCE_FILES
        ThreadPool/ThreadSafeQueue.hpp
        ThreadPool/ThreadPool.hpp
        )

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${THREADPOOL_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

# Tests
if (BUILD_GA3_TESTS)
    message(STATUS "Building ga2 with Tests")

    add_subdirectory(tests)
endif ()