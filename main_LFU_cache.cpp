//-----------------------------------------------------------------------------
//
//  LFU cache in C++: simple driver program
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

#include "LFU_cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
    int hits = 0;
    int n;
    size_t m;

    std::cin >> m >> n;
    if (std::cin.fail() or m < 0 or n < 0) {
        std::cout << "Incorrect input. "
        "The first line should contain two integers m and n (1 <= m <= 1e7 and 0 <= n <= 1e7). "
        "Where m is the size of cache and n is a number of elements that will try to cache. "
        "The second line should contain n integers a_1, a_2, ..., a_n (1 <= a_i <= 1e9) - id "
        "of elements.\n";
        return 1;
    }
    std::vector<int> queries(n);

    for (int i = 0; i < n; ++i) {
      std::cin >> queries[i];
    }

    caches::cache_t<int> c{m};
    for (int i = 0; i < n; ++i) {
        int q;
        q = queries[i];
        if (c.lookup_update(q, slow_get_page_int)) {
            hits += 1;
        #ifdef DEBUG_INFO
            std::cout << "was hit in" << std::endl;
        #endif
        }
        #ifdef DEBUG_INFO
            std::cout << q << ' ' << i << '\n';
            c.print_cache();
        #endif
    }
    
    std::cout << hits << std::endl;
}