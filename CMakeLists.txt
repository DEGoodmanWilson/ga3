cmake_minimum_required(VERSION 3.8)

project(ga3)

set(CMAKE_CXX_STANDARD 17)

include(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

include_directories(SYSTEM PRIVATE .)

set(SOURCE_FILES
        ga3/optional.h
        ga3/gene.hpp
        ga3/chromosome.hpp
        ga3/chromosome.cpp
        ga3/population.hpp
        ga3/population.cpp
        ga3/ga3.hpp
        )

set(THREADPOOL_SOURCE_FILES
        ThreadPool/ThreadSafeQueue.h
        ThreadPool/ThreadPool.h
        )

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${THREADPOOL_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

# Tests
#if (BUILD_LUNA_TESTS)
    message(STATUS "Building GA2 with Tests")

    add_subdirectory(tests)
#endif ()
