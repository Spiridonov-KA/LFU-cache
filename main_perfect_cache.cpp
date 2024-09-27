//-----------------------------------------------------------------------------
//
//  Perfect cache in C++: simple driver program
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

#include "perfect_cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
    int hits = 0;
    int n;
    size_t m;

    std::cin >> m >> n;
    std::vector<int> queries(n);

    for (int i = 0; i < n; ++i) {
      std::cin >> queries[i];
    }

    caches::perfect_cache_t<int> p_c{m, queries};
    std::cout << p_c.count_hits(slow_get_page_int) << '\n';
}