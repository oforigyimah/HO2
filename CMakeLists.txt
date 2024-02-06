cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(HO2 src/main.c)

# add_definitions(-D_WIN32_WINNT=0x0601)



#target_link_libraries(HO2
#         PUBLIC curl
#         PUBLIC ssl
#)
#target_include_directories(HO2
#         PUBLIC external/curl/include
#        PUBLIC external/openssl/include
#)
#
#target_link_directories(HO2
#         PUBLIC external/curl/lib
#        PUBLIC external/openssl/lib
#)





#target_link_libraries(HO2
#        PUBLIC C:\\curl-8.6.0_1-win64-mingw\\lib\\libcurl.a
#)

#target_include_directories(HO2
#        PUBLIC C:\\curl-8.6.0_1-win64-mingw\\include
#)

add_subdirectory(external/curl)
add_subdirectory(external/polarssl)
add_subdirectory(external/hash)
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