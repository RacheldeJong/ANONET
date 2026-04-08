#include "count.h"

const bool Count::are_equivalent(Graph* g, const int v1, const int v2){
    cache_pair val1 = compute_value(g, v1);
    cache_pair val2 = compute_value(g, v2);
    return are_equal(val1, val2);
}

const cache_pair Count::compute_value(Graph* g, const int v){
    std::map<int, int> value;
    int v_copy = v;

    g->get_neighborhood_nauty(subgraph, v_copy, distance);
    int nodes = subgraph->nv - 1;
    int edges = subgraph->nde/2 - nodes;
    value[N_POS_CACHE] = nodes;
    value[M_POS_CACHE] = edges;
    return std::make_pair(0, value);
}

const long Count::get_int_value(cache_pair val){
    return DIFF_MAX_NORMALIZED;
}


const long Count::compute_difference(Graph* g, const int v1, const int v2){
    cache_pair val1 = compute_value(g, v1);
    cache_pair val2 = compute_value(g, v2);
    return compute_difference(g, val1, val2);
}

const long Count::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    long n1 = val1.second[N_POS_CACHE];
    long m1 = val1.second[M_POS_CACHE];
    long diffn = abs(val1.second[N_POS_CACHE] - val2.second[N_POS_CACHE]);
    long diffm = abs(val1.second[M_POS_CACHE] - val2.second[M_POS_CACHE]);
    long diffn_normalized;
    long diffm_normalized;
    // Diff = 0 -> o% difference. Otherwise at least 1%
    // If n or m equals 0, diff_normalized = DIFF_MAX_NORMALIZED
    if(diffn == 0) diffn_normalized = 0;
    else if(n1 > 0) diffn_normalized = std::max(1.0, ceil((double)diffn / n1 * 100));
    else diffn_normalized == DIFF_MAX_NORMALIZED;

    if(diffm == 0) diffm_normalized = 0;
    else if(m1 > 0) diffm_normalized = std::max(1.0, ceil((double)diffm / m1 * 100));
    else diffm_normalized = DIFF_MAX_NORMALIZED;

    long diff_normalized = std::max(diffn_normalized, diffm_normalized);
    return diff_normalized;
}

// Given that a certain edge is deleted, get the set of nodes that are affected by the operation
const std::set<int> Count::get_nodes_affected(Graph* g, const int source, const int target){
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

const void Count::print_val_measure(cache_pair val){
    std::cout<<"["<<val.second[N_POS_CACHE] << ", "<<val.second[M_POS_CACHE]<<"]"<<std::endl;
}

const bool Count::are_equal(cache_pair val1, cache_pair val2){
    return val1.second[N_POS_CACHE] == val2.second[N_POS_CACHE]
        && val1.second[M_POS_CACHE == val2.second[M_POS_CACHE]];
}


