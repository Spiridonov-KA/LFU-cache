#------------------------------------------------------------------------------
#
# LFU cache project
#
#------------------------------------------------------------------------------


# The command to compile the LFU cache
# rm -rf build
# cmake -DCMAKE_BUILD_TYPE=Release -DLFU_CACHE_RESULT -S . -B build 
# cmake --build ./build
# env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test


# The command to compile the perfect cache
# rm -rf build
# cmake -DCMAKE_BUILD_TYPE=Release -DPERFECT_CACHE_RESULT -S . -B build 
# cmake --build ./build
# env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.11)
project("LFU-cache")

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PERFECT_CACHE_RESULT "performance perfect cache" OFF)
option(LFU_CACHE_RESULT "performance LFU cache" OFF)

set(SRCS 
    source/cache.cpp
)

set(TESTS ${CMAKE_SOURCE_DIR}/tests)

set(EXECUTABLE_FILE main)
set(BUILD_FOLDER build)

add_library(cachelib STATIC ${SRCS})
target_include_directories(cachelib PUBLIC include)

add_executable(${EXECUTABLE_FILE} main.cpp)
target_link_libraries(${EXECUTABLE_FILE} PUBLIC cachelib)

function(run_tests path_to_tests)
    file(GLOB testfiles "${path_to_tests}/*.dat")
    foreach(file ${testfiles})
        add_test(
                NAME ${file}
                COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./main"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        set_tests_properties(${file} PROPERTIES DEPENDS ${EXECUTABLE_FILE})
    endforeach()
endfunction()


if (PERFECT_CACHE_RESULT STREQUAL "ON")
    add_definitions(-DPERFECT_CACHE_RESULT)
    message(STATUS "Perfect cache was chosen")
    target_compile_definitions(${EXECUTABLE_FILE} PRIVATE PERFECT_CACHE)
    run_tests("tests/perfect_cache")
endif()

if (LFU_CACHE_RESULT STREQUAL "ON")
    add_definitions(-DLFU_CACHE_RESULT)
    message(STATUS "LFU cache was chosen")
    target_compile_definitions(${EXECUTABLE_FILE} PRIVATE PERFECT_CACHE)
    run_tests("tests/LFU_cache")
endif()