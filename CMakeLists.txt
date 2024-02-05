cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)

add_executable(HO2 src/main.c)

add_subdirectory(external/curl
                external/openssl
    )

target_link_libraries(HO2
        PUBLIC curl
        PUBLIC ssl
)
target_include_directories(HO2
        PUBLIC external/curl/include
        PUBLIC external/openssl/include
)

target_link_directories(HO2
        PUBLIC external/curl/lib
        PUBLIC external/openssl/lib
)
