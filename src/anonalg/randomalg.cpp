#include "randomalg.h"

std::vector< std::pair<int, int> > Anonrandom::select_edges(Graph *g, EquivalencePartition *EP, 
const int nr_edges){
    std::vector< std::pair<int, int> > selected_edges;
    auto edges = g->get_edges();

    // Return all edges left if nr_edges too large
    if(nr_edges >= edges.size()) return edges;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(edges.begin(), edges.end(), rng);

    // Add edges to vector
    for(auto edge : edges){
        selected_edges.push_back(edge);
        if(selected_edges.size() == nr_edges) break;
    }
    return selected_edges;
}
