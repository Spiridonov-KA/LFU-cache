## How to compile project
```bash
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build ./build
```
This creates to object files `./build/main_LFU_cache` and `./build/main_perfect_cache`

## Input
The first line should contain two integers m and n (1 <= m <= 1e7 and 0 <= n <= 1e7). Where m is the size of cache and n is a number of elements that will try to cache
The second line should contain n integers a_1, a_2, ..., a_n (1 <= a_i <= 1e9) - id of elements.

## Output
The program output a number of cache hits.

## Run tests
```bash
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

# How to add a new test
To add a new test for LFU cache algorithm, you need to create two files:
./tests/test_{k}.dat - file with data
./tests/test_{k}.ans - answer for this data
where k is any number

## LFU cache implementation
m - size of cache
- Insert element in cache: O(log(m))
- Delete element from cache: O(log(m))
- 
## Perfect cache implementation
m - size of cache
- Insert element in cache: O(1)
- Delete element from cache: O(m)