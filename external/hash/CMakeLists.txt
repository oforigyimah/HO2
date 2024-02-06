cmake_minimum_required(VERSION 3.10)
project(hash)

set(LIBNAME ${PROJECT_NAME})
set(TESTNAME tests)

file(GLOB SOURCES "lib/*.c")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${LIBNAME}
)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    add_executable(tests tests.c)
    target_link_libraries(tests ${PROJECT_NAME})
endif()