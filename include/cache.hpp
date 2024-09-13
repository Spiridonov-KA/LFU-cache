//-----------------------------------------------------------------------------
//
//  LFU cache header
//
//----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <list>
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
    std::unordered_map<size_t, std::unordered_set<KeyT>> cnt_hash_;
    std::multiset<size_t> cache_hits_;

public:
    cache_t(size_t sz) : sz_(sz) {}

    bool full() const { return (cache_.size() == sz_); }

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
private:
    void remove_elem_from_cache() {
        auto min_hits_iter = cache_hits_.begin();
        size_t min_hits = *min_hits_iter;
        KeyT key = *(cnt_hash_[min_hits].begin());
        cnt_hash_.erase(cnt_hash_.begin());
        cache_hits_.erase(min_hits_iter);
        cache_.erase(hash_[key]);
        hash_.erase(key);
    }

    template<typename F> 
    void add_elem_to_cache(KeyT key, F slow_get_page) {
        cache_.emplace_front(slow_get_page(key), 0);
        hash_.emplace(key, cache_.begin());
        cache_hits_.insert(NEW_ELEM);
        cnt_hash_[NEW_ELEM].insert(key);
    }

    void cache_hit(KeyT key) {
        auto it = hash_.find(key);
        cache_hits_.erase((*it).second->second);
        cnt_hash_[(*it).second->second].erase(key);
        ++(*it).second->second;
        cnt_hash_[(*it).second->second].insert(key);
        cache_hits_.insert((*it).second->second);
    }
};

}