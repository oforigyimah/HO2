cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)


if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

set(SOURCE_FILES "src/main.c" "src/helper_func/*.c")
set(CHILD_OHNO_FILES "src/childps/child_ohno_openssl.c" "src/childps/child_ohno_openssl_func_ed.c" "src/childps/child_ohno_polarssl.c" "src/childps/child_ohno_polarssl_func_ed.c")
set(HElPER_FUNCS "src/helper_func/*.c")

foreach(file ${CHILD_OHNO_FILES})
    string(REPLACE "src/childps/" "" target ${file})
    string(REPLACE ".c" "" target ${target})
    file(GLOB SOURCES ${file} "src/helper_func/*.c")
    add_executable(${target} ${SOURCES})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/childps")
endforeach()

file(GLOB HO2_SOURCES ${SOURCE_FILES})
file(GLOB HElPER_FUNC_FILES ${HElPER_FUNCS})
add_executable(HO2 ${HO2_SOURCES})
set_target_properties(HO2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(HO2Mark src/benchmark/main.c)
set_target_properties(HO2Mark PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/benchmark")

message("helper func files: ${HElPER_FUNC_FILES}")

add_executable(tests src/tests/tests.c ${HElPER_FUNC_FILES})
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests")


add_subdirectory(external/polarssl)
add_subdirectory(external/hash)

find_package(CURL REQUIRED)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    set(INCLUDE_DIRS C:\\curl-8.6.0_1-win64-mingw\\include external/polarssl/include external/hash/include C:\\Program Files\\OpenSSL-Win64\\include)
    set(LINK_DIRS C:\\curl-8.6.0_1-win64-mingw\\lib external/polarssl/lib external/hash/lib C:\\Program Files\\OpenSSL-Win64\\lib)
else()
    add_subdirectory(external/curl)
    set(INCLUDE_DIRS external/curl/include external/polarssl/include external/hash/include)
    set(LINK_DIRS external/curl/lib external/polarssl/lib external/hash/lib)
endif()

set(LINK_LIBS curl polarssl hash ssl crypto)
if(WIN32)
    list(APPEND LINK_LIBS Bcrypt.lib)
endif()

foreach(target HO2 HO2Mark tests child_ohno_openssl child_ohno_openssl_func_ed child_ohno_polarssl child_ohno_polarssl_func_ed)
    target_include_directories(${target} PUBLIC ${INCLUDE_DIRS})
    target_link_libraries(${target} PUBLIC ${LINK_LIBS})
    target_link_directories(${target} PUBLIC ${LINK_DIRS})
endforeach()