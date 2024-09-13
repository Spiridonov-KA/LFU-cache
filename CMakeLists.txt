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
    source/cache.cpp
)

set(TESTS ${CMAKE_SOURCE_DIR}/tests)

set(EXECUTABLE_FILE main)
set(BUILD_FOLDER build)

add_library(cachelib STATIC ${SRCS})
target_include_directories(cachelib PUBLIC include)

add_executable(${EXECUTABLE_FILE} main.cpp)
target_link_libraries(${EXECUTABLE_FILE} PUBLIC cachelib)

file(GLOB testfiles "tests/*.dat")
foreach(file ${testfiles})
    add_test(
            NAME ${file}
            COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./main"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(${file} PROPERTIES DEPENDS ${EXECUTABLE_FILE})
endforeach()