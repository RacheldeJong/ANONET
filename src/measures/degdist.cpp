#include "degdist.h"
#include "edgecount.h"
#include <functional>
#include <iomanip>

const cache_pair Degdist::compute_value(Graph* g, const int v){
    std::map< int, int> deg_dist;
    int empty_int = 0;

    if(distance == 0){
        deg_dist[g->get_degree(v)] = 1;
    }
    else{
        int v_copy = v;
        g->get_neighborhood_nauty(subgraph, v_copy, distance);
        
        for(int i=0; i < subgraph->nv; i++){
            int deg = subgraph->d[i];
            if(deg_dist.find(deg) != deg_dist.end()){
                deg_dist[deg]+=1;
            }
            else{
                deg_dist[deg]=1;
            }
        }
    }
    
    return std::make_pair(empty_int, deg_dist);
}

// Determine whether two nodes are equivalent
const bool Degdist::are_equivalent(Graph* g, const int v1, const int v2){
    auto val1 = compute_value(g, v1);
    auto val2 = compute_value(g, v2);
    return are_equal(val1, val2);
}

const long Degdist::compute_difference(Graph* g, const int v1, const int v2){
    std::map<int, int> empty_map = {};

    // Get degree distribution d-neighborhood
    auto val1 = compute_value(g, v1);
    auto val2 = compute_value(g, v2);
    
    return compute_difference(g, val1, val2);
}

const long Degdist::compute_difference(Graph* g, cache_pair val1, cache_pair val2){
    // Initialize values
    auto it1 = val1.second.crbegin();
    auto it2 = val2.second.crbegin();
    int deg1 = it1->first;
    int deg2 = it2->first;
    int count1 = it1->second;
    int count2 = it2->second;
    long diff = 0;

    // Compute difference
    while(it1 != val1.second.crend() && it2 != val2.second.crend()){

        if(count1 > count2){
            diff += count2 * abs(deg1 - deg2);
            count1 -= count2;
            it2++; deg2 = it2->first; count2 = it2->second;
        }
        else if(count2 > count1){
            diff += count1 * abs(deg1 - deg2);
            count2 -= count1;
            it1++; deg1 = it1->first; count1 = it1->second;
        }
        else{
            diff += count1 * abs(deg1 - deg2);
            it1++; deg1 = it1->first; count1 = it1->second;
            it2++; deg2 = it2->first; count2 = it2->second;
        }
    }

    // Remainder difference; if out of values
    while(it1 != val1.second.crend()){
        diff += it1->first * count1;
        it1++;
        count1 = it1->second;
    }

    while(it2 != val2.second.crend()){
        diff += it2->first * count2;
        it2++;
        count2 = it2->second;
    }

    return diff;
}

const std::set<int> Degdist::get_nodes_affected(Graph* g, const int source, const int target){
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

const void Degdist::print_val_measure(const cache_pair val){
    std::map< int, int> val_map = val.second;
    for(auto it : val_map){
        std::cout<<"["<<it.first<<", "<< it.second<<"]"<<", ";
    }
    std::cout<<std::endl;
}

const bool Degdist::are_equal(const cache_pair val1, const cache_pair val2){
    return val1.second == val2.second;
}
