cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib")
set(BUILD_SHARED_LIBS OFF)

add_compile_options(-fno-pic)


if (WIN32)
#    set(CMAKE_EXE_LINKER_FLAGS "-static")
    add_definitions(-DCURL_STATICLIB)
endif ()

set(SOURCE_FILES "src/main.c" "src/helper_func/*.c")
set(CHILD_OHNO_FILES "src/childps/child_ohno_openssl.c")
set(HElPER_FUNCS "src/helper_func/*.c")

foreach(file ${CHILD_OHNO_FILES})
    string(REPLACE "src/childps/" "" target ${file})
    file(GLOB SOURCES ${file} "src/helper_func/*.c")
    add_executable(ho2_child ${SOURCES})
    set_target_properties(ho2_child PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
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


add_subdirectory(external/hash)


if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    set(INCLUDE_DIRS external/hash/include "C:\\msys64\\mingw64\\include")
    set(LINK_DIRS external/hash/lib "C:\\msys64\\mingw64\\lib")
else()
    add_subdirectory(external/curl)
    set(INCLUDE_DIRS external/curl/include external/hash/include)
    set(LINK_DIRS external/curl/lib  external/hash/lib)
endif()

set(LINK_LIBS curl hash crypto ssl Jansson)
if(WIN32)
    list(APPEND LINK_LIBS Bcrypt.lib)
endif()

foreach(target HO2 HO2Mark tests ho2_child)
    target_include_directories(${target} PUBLIC ${INCLUDE_DIRS})
    target_link_libraries(${target} PUBLIC ${LINK_LIBS})
    target_link_directories(${target} PUBLIC ${LINK_DIRS})
    target_compile_options(${target} PUBLIC -Ofast)
endforeach()