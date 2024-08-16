#include "none.h"

const cache_pair None::compute_value(Graph* g, const int v){
    std::map<int, int> empty_map = {};
    int empty_int = 0; 
    return std::make_pair(empty_int, empty_map);
}

// Determine whether two nodes are equivalent
const bool None::are_equivalent(Graph* g, const int v1, const int v2){
    return true;
}

const long None::compute_difference(Graph* g, const int v1, const int v2){
    return 0;
}

const long None::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    return 0;
}

// TODO: here or somewhere else?
// Given that a certain edge is deleted, get the set of nodes that are affected by the operation
const std::set<int> None::get_nodes_affected(Graph* g, const int source, const int target){
    std::set<int> nodes_affected = {};
    return nodes_affected;
}

const void None::print_val_measure(const cache_pair val){
    std::cout<<val.first<<std::endl;
}

const bool None::are_equal(const cache_pair val1, const cache_pair val2){
    return val1.first == val2.first;
}
