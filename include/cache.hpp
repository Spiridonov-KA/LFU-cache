#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

template<typename T, typename KeyT = int>
class cache_t {
private:
    size_t sz_;
    std::vector<T> cache_;
public:
    cache_t(size_t sz) : sz_(sz) {}
    int cache(T);
};

int print();