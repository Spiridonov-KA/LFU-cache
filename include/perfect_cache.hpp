#pragma once

#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace caches {
    template<typename T, typename KeyT = int>
class perfect_cache_t {
private:
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;
    std::vector<KeyT> queries_;
    std::unordered_map<KeyT, std::queue<size_t>> keys_pos_;
    std::list<std::pair<KeyT, T>> cache_;
    size_t hits_ = 0;
    size_t sz_ = 0;
public:
    perfect_cache_t(size_t sz, std::vector<KeyT> &queries) : sz_(sz), queries_(queries) {
        for (size_t i = 0; i < queries_.size(); ++i) {
            keys_pos_[queries_[i]].push(i);
        }
    }

    bool full() const { return (cache_.size() == sz_ + 1); }

    template<typename F>
    size_t count_hits(F slow_get_page) {
        for (size_t i = 0; i < queries_.size(); ++i) {
            hits_ += lookup_update(queries_[i], slow_get_page, i);
        }
        return hits_;
    }

    // Implements a simulation of requesting an element by key to the cache. 
    // If the item is in the cache, then true is returned. 
    // If the element is not in the cache, then some element 
    // is deleted from the cache, the slow_get_page function 
    // is called and this element is inserted into the 
    // cache and false is returned
    template<typename F>
    bool lookup_update(KeyT key, F slow_get_page, int idx) {
        auto hit = hash_.find(key);
        if (!keys_pos_[key].empty() and keys_pos_[key].front() == idx ) {
            keys_pos_[key].pop();
        }
        if (hit == hash_.end()) {
            add_elem_to_cache(key, slow_get_page);
            if (full()) {
                remove_elem_from_cache();
            }
            return false;
        }
        return true;
    }

    // Searches for the key that is farthest in the request
    KeyT find_farthest_key() {
        size_t farthest = 0;
        KeyT ans_key;
        for (auto it : cache_) {
            auto curr_key = it.first;
            if (!keys_pos_[curr_key].empty()) {
                if (keys_pos_[curr_key].front() > farthest) {
                    farthest = keys_pos_[curr_key].front();
                    ans_key = it.first;
                }
            }
            else {
                ans_key = it.first;
                return ans_key;
            }
        }
        keys_pos_[ans_key].pop();
        return ans_key;
    }

    // Implements deleting an item from the cache
    void remove_elem_from_cache() {
        KeyT key = find_farthest_key();
        cache_.erase(hash_[key]);
        hash_.erase(key);
    }

    // Implements the insertion of an element into the cache 
    // when there is free space for it
    template<typename F> 
    void add_elem_to_cache(KeyT key, F slow_get_page) {
        cache_.emplace_front(key, slow_get_page(key));
        hash_.emplace(key, cache_.begin());
        assert(cache_.size() <= sz_ + 1);
    }
};
}
