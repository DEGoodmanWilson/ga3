enable_testing()

include_directories(SYSTEM PRIVATE ..)
add_executable(${PROJECT_NAME}_tests
        main.cpp
        chromosome.cpp)

target_link_libraries(${PROJECT_NAME}_tests ${CONAN_LIBS})

add_test(${PROJECT_NAME}_tests ${PROJECT_NAME}_tests)
