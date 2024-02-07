cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

add_executable(HO2 src/main.c src/helper_func/string_helper.c)
add_executable(HO2Mark src/benchmark/main.c)
add_executable(child_ohno_openssl src/childps/child_ohno_openssl.c src/helper_func/string_helper.c src/helper_func/files_helper.c)

add_subdirectory(external/polarssl)
add_subdirectory(external/hash)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)

    target_include_directories(HO2
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\include
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\include
    )

    target_link_libraries(HO2
            PUBLIC curl
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
            PUBLIC Bcrypt.lib
    )


    target_link_directories(HO2
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\lib
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\lib
    )


else()
    add_subdirectory(external/curl)

        target_link_directories(HO2
            PUBLIC external/curl/lib
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
    )
    target_include_directories(HO2
            PUBLIC external/curl/include
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
    )

    target_link_libraries(HO2
            PUBLIC curl
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
    )

    target_link_libraries(HO2Mark
            PUBLIC curl
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
    )

    target_include_directories(HO2Mark
            PUBLIC external/curl/include
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
    )

    target_link_directories(HO2Mark
            PUBLIC external/curl/lib
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
    )

    target_link_libraries(child_ohno_openssl
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
    )

    target_include_directories(child_ohno_openssl
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
    )

    target_link_directories(child_ohno_openssl
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
    )




endif()