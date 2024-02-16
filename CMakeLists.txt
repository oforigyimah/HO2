cmake_minimum_required(VERSION 3.22)
project(HO2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib")
set(BUILD_SHARED_LIBS OFF)

if (WIN32)
#    set(CMAKE_EXE_LINKER_FLAGS "-static")
    add_definitions(-DCURL_STATICLIB)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app.rc")
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

if (WIN32)
    add_executable(HO2 ${HO2_SOURCES} ${APP_ICON_RESOURCE_WINDOWS})
else()
add_executable(HO2 ${HO2_SOURCES})
endif()
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


set(LINK_LIBS curl hash crypto ssl jansson)
if(WIN32)
    list(APPEND LINK_LIBS Bcrypt.lib)
endif()
add_compile_options(-fPIE)
foreach(target HO2 HO2Mark tests ho2_child)
    target_include_directories(${target} PUBLIC ${INCLUDE_DIRS})
    target_link_libraries(${target} PUBLIC ${LINK_LIBS})
    target_link_directories(${target} PUBLIC ${LINK_DIRS})
    target_compile_options(${target} PUBLIC -Ofast)
    # Set the RPATH
    set_target_properties(${target} PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "$ORIGIN"
            )
endforeach()

if (WIN32)
    # Define the patterns to match
    set(PATTERNS libcurl libhash libcrypto libssl libJansson libBcrypt)

    # Iterate over each directory
    foreach(DIR ${LINK_DIRS})
        # Iterate over each pattern
        foreach(PATTERN ${PATTERNS})
            # Get a list of all files that match the pattern
            file(GLOB_RECURSE MATCHING_LIBS "${DIR}/*${PATTERN}*")

            # Check if the list is empty
            if(NOT MATCHING_LIBS)
                message("No matching files found for pattern: ${PATTERN} in directory: ${DIR}")
            else()
                # Iterate over each matching file
                foreach(LIB ${MATCHING_LIBS})
                    # Print the file path
                    message("Copying file: ${LIB}")

                    # Copy the file to the lib directory
                    file(COPY ${LIB} DESTINATION ${CMAKE_BINARY_DIR}/lib)
                endforeach()
            endif()
        endforeach()
    endforeach()

endif()