//-----------------------------------------------------------------------------
//
//  LFU cache in C++: simple driver program
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

#include "LFU_cache.hpp"
#include "perfect_cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

void run_LFU_cache(std::vector<int> &queries, size_t m) {
  int hits = 0;
  int n = queries.size();
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

int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;
  std::vector<int> queries(n);

  for (int i = 0; i < n; ++i) {
    std::cin >> queries[i];
  }

#ifdef LFU_CACHE_RESULT
  run_LFU_cache(queries, m);
#endif

#ifdef PERFECT_CACHE_RESULT
  caches::perfect_cache_t<int> p_c{m, queries};
  std::cout << p_c.count_hits(slow_get_page_int) << '\n';
#endif
}