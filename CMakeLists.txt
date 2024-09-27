#------------------------------------------------------------------------------
#
# LFU cache project
#
#------------------------------------------------------------------------------


# rm -rf build
# cmake -DCMAKE_BUILD_TYPE=Release -S . -B build 
# cmake --build ./build
# env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.11)
project("LFU-cache")

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS 
    source/perfect_cache.cpp
    source/LFU_cache.cpp
)

set(TESTS ${CMAKE_SOURCE_DIR}/tests)

set(EXECUTABLE_FILE_LFU_CACHE main_LFU_cache)
set(EXECUTABLE_FILE_PERFECT_CACHE main_perfect_cache)
set(BUILD_FOLDER build)

add_library(cachelib STATIC ${SRCS})
target_include_directories(cachelib PUBLIC include)

add_executable(${EXECUTABLE_FILE_LFU_CACHE} main_LFU_cache.cpp)
target_link_libraries(${EXECUTABLE_FILE_LFU_CACHE} PUBLIC cachelib)

add_executable(${EXECUTABLE_FILE_PERFECT_CACHE} main_perfect_cache.cpp)
target_link_libraries(${EXECUTABLE_FILE_PERFECT_CACHE} PUBLIC cachelib)

function(run_tests path_to_tests exe_file)
    file(GLOB testfiles "${path_to_tests}/*.dat")
    foreach(file ${testfiles})
        add_test(
                NAME ${file}
                COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./${exe_file}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        set_tests_properties(${file} PROPERTIES DEPENDS ${exe_file})
    endforeach()
endfunction()

run_tests("tests/LFU_cache" ${EXECUTABLE_FILE_LFU_CACHE})
run_tests("tests/perfect_cache" ${EXECUTABLE_FILE_PERFECT_CACHE})