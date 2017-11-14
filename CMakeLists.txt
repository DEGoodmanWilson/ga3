cmake_minimum_required(VERSION 3.8)
set(PROJECT_NAME ga2)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

include(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

set(SOURCE_FILES ga2.h ga2Chromosome.h ga2Chromosome.cpp ga2Gene.h ga2Population.h ga2Population.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

# Tests
# add_subdirectory(tests)
