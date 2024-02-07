cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

file(GLOB HO2_SOURCES
        "src/main.c"
        "src/helper_func/*.c"
)

file(GLOB CHILD_OHNO_OPENSSL_SOURCES
        "src/childps/child_ohno_openssl.c"
        "src/helper_func/*.c"
)

file(GLOB CHILD_OHNO_OPENSSL_FUNC_ED_SOURCES
        "src/childps/child_ohno_openssl_func_ed.c"
        "src/helper_func/*.c"
)


add_executable(HO2 ${HO2_SOURCES})
set_target_properties(HO2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(HO2Mark src/benchmark/main.c)
set_target_properties(HO2Mark PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/benchmark")

add_executable(child_ohno_openssl ${CHILD_OHNO_OPENSSL_SOURCES})
set_target_properties(child_ohno_openssl PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/childps")

add_executable(child_ohno_openssl_func_ed ${CHILD_OHNO_OPENSSL_FUNC_ED_SOURCES})
set_target_properties(child_ohno_openssl_func_ed PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/childps")


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

    target_link_libraries(HO2Mark
            PUBLIC curl
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
            PUBLIC Bcrypt.lib
    )

    target_include_directories(HO2Mark
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\include
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\include
    )

    target_link_directories(HO2Mark
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\lib
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\lib
    )

    target_link_libraries(child_ohno_openssl
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
            PUBLIC Bcrypt.lib
    )

    target_include_directories(child_ohno_openssl
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\include
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\include
    )

    target_link_directories(child_ohno_openssl
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\lib
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\lib
    )


    target_link_libraries(child_ohno_openssl_func_ed
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
            PUBLIC Bcrypt.lib
    )

    target_include_directories(child_ohno_openssl_func_ed
            PUBLIC C:\\curl-8.6.0_1-win64-mingw\\include
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
            PUBLIC C:\\Program Files\\OpenSSL-Win64\\include
    )

    target_link_directories(child_ohno_openssl_func_ed
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


    target_link_libraries(child_ohno_openssl_func_ed
            PUBLIC polarssl
            PUBLIC hash
            PUBLIC ssl
            PUBLIC crypto
    )

    target_include_directories(child_ohno_openssl_func_ed
            PUBLIC external/polarssl/include
            PUBLIC external/hash/include
    )

    target_link_directories(child_ohno_openssl_func_ed
            PUBLIC external/polarssl/lib
            PUBLIC external/hash/lib
    )


endif()