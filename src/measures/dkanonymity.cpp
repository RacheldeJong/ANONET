#include "dkanonymity.h"

const bool dkAnonymity::are_equivalent(Graph* g, const int v1, const int v2){
    cache_pair val1 = compute_value(g, v1);
    cache_pair val2 = compute_value(g, v2);
    return are_equal(val1, val2);
}

const cache_pair dkAnonymity::compute_value(Graph* g, const int v){
    int n = g->get_number_nodes();
    int m = g->get_number_edges();
    std::map<int, int> empty_map;

    static DEFAULTOPTIONS_SPARSEGRAPH(options);
    options.getcanon = TRUE;

    // Get neighborhood
    int copy_v = v;
    g->get_neighborhood_nauty(subgraph, copy_v, distance);

    // Compute canonical labeling
    sparsenauty(subgraph, lab, ptn, orbits, &options, &stats, cansubgraph);
    
    // Make hash
    int can_hash = hashgraph_sg(cansubgraph, HASHKEY);

    return std::make_pair(can_hash, empty_map);
}

const long dkAnonymity::compute_difference(Graph* g, const int v1, const int v2){
    std::map< int, int> emptymap = {};
    int empty_int = -1;
    // TODO: implement
    return 0;
}

const long dkAnonymity::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    long val = abs(val1.first - val2.first); 
    return val;
}

const std::set<int> dkAnonymity::get_nodes_affected(Graph* g, const int source, const int target){
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

const void dkAnonymity::print_val_measure(const cache_pair val){
    std::cout<<val.first<<std::endl;
}

const bool dkAnonymity::are_equal(const cache_pair val1, const cache_pair val2){
    return val1 == val2;
}
