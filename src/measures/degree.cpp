#include "degree.h"
#include <functional>
#include <iomanip>

const cache_pair Degree::compute_value(Graph* g, const int v){
    std::map< int, int> empty_map;
    int degree = g->get_degree(v);
    return std::make_pair(degree, empty_map);
}

const bool Degree::are_equivalent(Graph* g, const int v1, const int v2){
    auto val1 = compute_value(g, v1);
    auto val2 = compute_value(g, v2);
    return are_equal(val1, val2);
}

const long Degree::compute_difference(Graph* g, const int v1, const int v2){
    auto val1 = compute_value(g, v1);
    auto val2 = compute_value(g, v2);
    return compute_difference(g, val1, val2);
}

const long Degree::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    if(val1 == val2) return 0;
    return DIFF_MAX_NORMALIZED;
}

const std::set<int> Degree::get_nodes_affected(Graph* g, const int source, const int target){
    std::set<int> nodes_affected;
    nodes_affected.insert(source);
    nodes_affected.insert(target);
    return nodes_affected;
}

const void Degree::print_val_measure(const cache_pair val){
    std::map< int, int> val_map = val.second;
    for(auto it : val_map){
        std::cout<<"["<<it.first<<", "<< it.second<<"]"<<", ";
    }
    std::cout<<std::endl;
}

const bool Degree::are_equal(const cache_pair val1, const cache_pair val2){
    return val1.second == val2.second;
}


