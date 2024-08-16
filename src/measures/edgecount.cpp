#include "edgecount.h"

const bool Edgecount::are_equivalent(Graph* g, const int v1, const int v2){
    cache_pair val1 = compute_value(g, v1);
    cache_pair val2 = compute_value(g, v2);
    return are_equal(val1, val2);
}

const cache_pair Edgecount::compute_value(Graph* g, const int v){
    std::map<int, int> emptymap;
    int v_copy = v;

    g->get_neighborhood_nauty(subgraph, v_copy, distance);
    int edges = subgraph->nde;
    return std::make_pair(edges, emptymap);
}

const long Edgecount::compute_difference(Graph* g, const int v1, const int v2){
    std::map<int, int> empty_map;
    cache_pair val1 = compute_value(g, v1);
    cache_pair val2 = compute_value(g, v2);
    long diff = abs(val1.first - val2.first);
    return diff;
}

const long Edgecount::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    std::map<int, int> empty_map;
    long diff = abs(val1.first - val2.first);
    return diff;
}

// Given that a certain edge is deleted, get the set of nodes that are affected by the operation
const std::set<int> Edgecount::get_nodes_affected(Graph* g, const int source, const int target){
    std::set<int> nodes_affected;
    int _;
    nodes_affected.insert(source);
    nodes_affected.insert(target);
    auto s1 = g->get_neighborhood_nodes(source, distance, _);
    auto s2 = g->get_neighborhood_nodes(target, distance, _);
    
    for(auto node : s1){
        if(s2.find(node) != s2.end()){
            nodes_affected.insert(node);
        }
    }
    return nodes_affected;
}

const void Edgecount::print_val_measure(cache_pair val){
    std::cout<<val.first<<std::endl;
}

const bool Edgecount::are_equal(cache_pair val1, cache_pair val2){
    return abs(val1.first - val2.first);
}


