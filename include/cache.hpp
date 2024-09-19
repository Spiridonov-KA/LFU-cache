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
    void remove_elem_from_cache() {
        // Итератор на значение елемента с минимальным hit'ом
        auto min_hits_iter = all_hits.begin();
        // Минимальное кол-во hit'ов в кэше
        size_t min_hits = *min_hits_iter;
        // Ключ элемента, который имеет минимальное кол-во hit'ов
        assert(elems_with_this_hit[min_hits].size() > 0);
        KeyT key = *(elems_with_this_hit[min_hits].begin());
        // Удаляем этот ключ из elems_with_this_hit
        elems_with_this_hit[min_hits].erase(elems_with_this_hit[min_hits].begin());
        // Уменьшаем минимальное кол-во hit'ов на единицу
        all_hits.erase(min_hits_iter);
        // Удаляем элемент с ключом key из кэша
        assert(hash_.find(key) != hash_.end());
        cache_.erase(hash_[key]);
        // Удаляем элемент с ключом key из хэша
        hash_.erase(key);
    }

    template<typename F> 
    void add_elem_to_cache(KeyT key, F slow_get_page) {
        cache_.emplace_front(slow_get_page(key), 0);
        hash_.emplace(key, cache_.begin());
        all_hits.insert(NEW_ELEM);
        elems_with_this_hit[NEW_ELEM].insert(key);
    }

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

    bool full() const { return (cache_.size() == sz_); }

    template<typename F>
    size_t count_hits(F slow_get_page) {
        for (size_t i = 0; i < queries_.size(); ++i) {
            hits_ += lookup_update(queries_[i], slow_get_page, i);
        }
        return hits_;
    }

    template<typename F>
    bool lookup_update(KeyT key, F slow_get_page, int idx) {
        auto hit = hash_.find(key);
        if (!keys_pos_[key].empty() and keys_pos_[key].front() == idx ) {
            keys_pos_[key].pop();
        }
        if (hit == hash_.end()) {
            if (full()) {
                remove_elem_from_cache();
            }
            add_elem_to_cache(key, slow_get_page);
            return false;
        }
        return true;
    }

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

    void remove_elem_from_cache() {
        KeyT key = find_farthest_key();
        cache_.erase(hash_[key]);
        hash_.erase(key);
    }

    template<typename F> 
    void add_elem_to_cache(KeyT key, F slow_get_page) {
        cache_.emplace_front(key, slow_get_page(key));
        hash_.emplace(key, cache_.begin());
        // keys_pos_[key].pop();
    }

    void cache_hit(KeyT key) {
        keys_pos_[key].pop();
    }
};

}