#include "unique_only.h"
#include <random>

std::vector< std::pair<int, int> > AnonUnique::select_edges(Graph *g, EquivalencePartition *EP, 
const int nr_edges){
    std::vector< std::pair<int, int> > unique_edges, selected_edges;
    auto edges = g->get_edges();
    std::pair<int, int> edge;
    std::set<int> edge_nrs;
    int edge_nr;
    
    // Return all edges left if nr_edges too large
    if(nr_edges >= edges.size()) return edges;

    auto unique_nodes = EP->get_k_anonymous_nodes(k-1, false);

    // Find edges that are connected to at least one unique node
    for(auto edge : edges){
        switch (variant)
        {
            case VARIANT_UU:        
                if (unique_nodes.find(edge.first) != unique_nodes.end() &&
                unique_nodes.find(edge.second) != unique_nodes.end()){
                    unique_edges.push_back(edge);
                }
                break;
            case VARIANT_U:
            default:
                if (unique_nodes.find(edge.first) != unique_nodes.end() ||
                unique_nodes.find(edge.second) != unique_nodes.end()){
                    unique_edges.push_back(edge);
                }
                break;
        }
    }

    std::set<std::pair<int, int>> chosen_edges;
    std::random_device rd;
    std::mt19937 rng(rd());
    // Set of edges too large; return edges + random edges
    if(nr_edges >= unique_edges.size()){
        selected_edges =  unique_edges;
        std::copy(unique_edges.begin(), unique_edges.end(), std::inserter(chosen_edges, chosen_edges.begin()));
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(edges.begin(), edges.end(), rng);
        for(auto edge : edges){
            int elements_before = selected_edges.size();
            chosen_edges.insert(edge);
            if(elements_before == chosen_edges.size()) continue;
            selected_edges.push_back(edge);
            if(selected_edges.size() == nr_edges) break;
        }
        return selected_edges;
    }

    std::shuffle(unique_edges.begin(), unique_edges.end(), rng);
    // Get edges to select
    for(auto edge : unique_edges){
        selected_edges.push_back(edge);
        if(selected_edges.size() == nr_edges)
            break;
    }
    return selected_edges;
}