#include "cache.h"

Cache::~Cache(){
}

int Cache::get_cache_size(){
    return cache.size();
}

cache_pair Cache::get_cache_entry(const int v){
    if(cache.find(v) == cache.end()){
        auto val = eq_measure.compute_value(g, v);
        vals_computed++;
        cache.insert(std::make_pair(v, val));
        return val;
    }
    return cache[v];
}

cache_pair Cache::get_cache_entry_computed(const int v){
    if(cache.find(v) == cache.end())
        return EMPTY_CACHE_PAIR;
    return cache[v];
}

void Cache::update_cache_entry(const int v){
    auto val = eq_measure.compute_value(g, v);
    auto it = cache.find(v);
    if(it != cache.end()) it->second = val;
    else cache.insert(std::make_pair(v, val));
}

const bool Cache::are_equivalent(const int v1, const int v2){
    auto val1 = get_cache_entry(v1);
    auto val2 = get_cache_entry(v2);
    return val1 == val2;
}

const void Cache::print_val_measure(const cache_pair val){
    if(cache_type == CACHE_TYPE_INT) print_val_measure(val.first);
    else print_val_measure(val.second);
}

const void Cache::print_val_measure(const int val){
    std::cout<<val;
}

const void Cache::print_val_measure(const std::map<int, int> val){
    for(auto it : val){
        std::cout<<"["<<it.first<<", "<<it.second<<"]";
    }
}

const void Cache::print_val_node(const int v){
    auto val = get_cache_entry(v);
    print_val_measure(val);
}

const bool Cache::are_equal(const cache_pair val1, cache_pair val2){
    return val1 == val2;
}

const int Cache::get_vals_computed(){
    return vals_computed;
}

const void Cache::print_statistics(){
    std::cout<<"Cache size: "<<get_cache_size()<<std::endl;
    std::cout<<"Values computed: "<<vals_computed<<std::endl;
    std::cout<<"Values obtained from cache: "<<vals_from_cache<<std::endl;
}
