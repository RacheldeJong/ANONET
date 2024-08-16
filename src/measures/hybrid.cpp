#include "hybrid.h"
#include <functional>
#include <iomanip>

const cache_pair Hybrid::compute_value(Graph* g, const int v){
    int empty_int = 0;
    std::map< int, int> deg_dist;
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

    if(distance == 0){
        deg_dist[g->get_degree(v)] = 1;
    }
    else{
        int nr_edges;
        auto nodes_all = g->get_neighborhood_nodes(v, distance, nr_edges);
        auto nodes_remove = g->get_neighborhood_nodes(v, distance-1, nr_edges);
        std::set<int> nodes_fringe;
        for(auto node : nodes_all){
            if(nodes_remove.find(node)== nodes_remove.end()){
                int degree = g->get_degree(node);
                if(deg_dist.find(degree) != deg_dist.end()){
                    deg_dist[degree] += 1;
                }
                else{
                    deg_dist[degree] = 1;
                }
            }
        }
    }
    return std::make_pair(can_hash, deg_dist);
}

const bool Hybrid::are_equivalent(Graph* g, const int v1, const int v2){
    cache_pair val1 = compute_value(g, v1);
    cache_pair val2 = compute_value(g, v2);
    return are_equal(val1, val2);
}

// TODO
const long Hybrid::compute_difference(Graph* g, const int v1, const int v2){
    return 0;
}

const long Hybrid::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    // TODO
    return 0;
}

// Given that a certain edge is deleted, get the set of nodes that are affected by the operation
const std::set<int> Hybrid::get_nodes_affected(Graph* g, const int source, const int target){
    std::set<int> nodes_affected;
    int _;
    nodes_affected.insert(source);
    nodes_affected.insert(target);
    auto s1 = g->get_neighborhood_nodes(source, distance, _);
    auto s2 = g->get_neighborhood_nodes(target, distance, _);
    
    for(auto node : s1) nodes_affected.insert(node);
    for(auto node : s2) nodes_affected.insert(node);
    return nodes_affected;
}

const void Hybrid::print_val_measure(const cache_pair val){
    auto val_map = val.second;
    std::cout<<val.first<<", ";
    for(auto it : val_map){
        std::cout<<"["<<it.first<<", "<< it.second<<"], ";
    }
    std::cout<<std::endl;
}

const bool Hybrid::are_equal(const cache_pair val1, const cache_pair val2){
    return val1 == val2;
}


