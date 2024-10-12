//-----------------------------------------------------------------------------
//
//  LFU cache header
//
//----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace caches {

// Every new added element has 0 hits
const size_t NEW_ELEM = 0;

template <typename T, typename KeyT = int> 
class cache_t {
private:
    size_t sz_;

    using ListIt = typename std::list<std::pair<T, size_t>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;
    std::list<std::pair<T, size_t>> cache_;
    // std::unordered_map<size_t, std::unordered_set<KeyT>> elems_with_this_hit;
    std::multiset<std::pair<size_t, KeyT>> all_hits;

public:
    cache_t(size_t sz) : sz_(sz) {}

    bool full() const { return (cache_.size() == sz_); }


    // Implements a simulation of requesting an element by key to the cache. 
    // If the item is in the cache, then true is returned. 
    // If the element is not in the cache, then some element 
    // is deleted from the cache, the slow_get_page function 
    // is called and this element is inserted into the 
    // cache and false is returned
    template <typename F> 
    bool lookup_update(KeyT key, F slow_get_page) {
		if (sz_ == 0) return false;
        auto hit = hash_.find(key);
        if (hit == hash_.end()) {
            if (full()) {
                remove_elem_from_cache();
            }
            add_elem_to_cache(key, slow_get_page);
            assert(hash_.size() <= sz_ and cache_.size() <= sz_ and all_hits.size() <= sz_);
            return false;
        }
        cache_hit(key);
        return true;
    }

    void print_cache() const {
        for (auto it : cache_) {
            std::cout << it.first << ' ';
        }
        std::cout << std::endl;
    }
private:
    // Implements deleting an item from the cache
    void remove_elem_from_cache() {
        // Iterator to the value of the element with the minimum hit
        auto min_hits_iter = all_hits.begin();
        // Minimum number of hits in the cache
        size_t min_hits = (*min_hits_iter).first;
        KeyT key = (*min_hits_iter).second;
        // Reducing the minimum number of hits by one
        all_hits.erase(min_hits_iter);
        // Deleting the element with the key "key" from the cache
        assert(hash_.find(key) != hash_.end());
        cache_.erase(hash_[key]);
        // Removing the element with the key "key" from the hash
        hash_.erase(key);
    }

    // Implements the insertion of an element into the cache 
    // when there is free space for it
    template<typename F> 
    void add_elem_to_cache(KeyT key, F slow_get_page) {
        cache_.emplace_front(slow_get_page(key), 0);
        hash_.emplace(key, cache_.begin());
        all_hits.insert({NEW_ELEM, key});
    }

    // Increases the hit counter of the element
    void cache_hit(KeyT &key) {
        auto it = hash_.find(key);
        assert(it != hash_.end());
        size_t n_hits = (*it).second->second;
        all_hits.extract({n_hits, key});
        ++n_hits;
        all_hits.insert({n_hits, key});
    }
};

}
