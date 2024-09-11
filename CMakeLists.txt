#------------------------------------------------------------------------------
#
# LFU cache project
#
#------------------------------------------------------------------------------

# rm -rf build
# cmake -DCMAKE_BUILD_TYPE=Release -S . -B build 
# cmake --build ./build
# env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

cmake_minimum_required(VERSION 3.11)
project("LRU-cache")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS 
    source/cache.cpp
)

add_library(cachelib STATIC ${SRCS})
target_include_directories(cachelib PUBLIC include)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC cachelib)