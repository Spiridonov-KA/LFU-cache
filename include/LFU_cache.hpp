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
    std::unordered_map<size_t, std::unordered_set<KeyT>> elems_with_this_hit;
    std::multiset<size_t> all_hits;

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
        auto hit = hash_.find(key);
        if (hit == hash_.end()) {
            if (full()) {
                remove_elem_from_cache();
            }
            add_elem_to_cache(key, slow_get_page);
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
        size_t min_hits = *min_hits_iter;
        // The key of the element that has the minimum number of hits
        assert(elems_with_this_hit[min_hits].size() > 0);
        KeyT key = *(elems_with_this_hit[min_hits].begin());
        // Removing this key from elems_with_this_hit
        elems_with_this_hit[min_hits].erase(elems_with_this_hit[min_hits].begin());
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
        all_hits.insert(NEW_ELEM);
        elems_with_this_hit[NEW_ELEM].insert(key);
    }

    // Increases the hit counter of the element
    void cache_hit(KeyT key) {
        auto it = hash_.find(key);
        assert(it != hash_.end());
        all_hits.extract((*it).second->second);
        assert(elems_with_this_hit[(*it).second->second].count(key) == 1);
        elems_with_this_hit[(*it).second->second].erase(key);
        ++(*it).second->second;
        assert(elems_with_this_hit[(*it).second->second].count(key) == 0);
        elems_with_this_hit[(*it).second->second].insert(key);
        all_hits.insert((*it).second->second);
    }

};

}