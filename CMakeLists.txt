cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)

add_executable(HO2 src/main.c)

add_subdirectory(external/curl)

target_link_libraries(HO2 PUBLIC curl)
target_include_directories(HO2 PUBLIC external/curl/include)

target_link_directories(HO2 PUBLIC external/curl/lib)

