#include "affected_unique.h"

std::vector< std::pair<int, int> > AnonAffectedUnique::select_edges(Graph *g, EquivalencePartition *EP, 
const int nr_edges){
    std::vector< std::pair<int, int> > selected_edges;
    auto edges = g->get_edges();
    std::vector<double> affected;
    std::vector<double> probabilities;
    Measure m = EP->cache.eq_measure;

    // Return all edges left if nr_edges too large
    if(nr_edges >= edges.size()) return edges;

    auto unique_nodes = EP->get_k_anonymous_nodes(k-1, false);

    std::set<std::pair<int, int>> chosen_edges;
    std::random_device rd;
    std::mt19937 rng(rd());
    
    double sum_affected = 0.0;
    std::set<int> result, aff_set;
    std::set<int> chosen;

    for(auto edge : edges){
        result = {};
        aff_set = m.get_nodes_affected(g, edge.first, edge.second);
        // Overlap unique nodes, aff_set
        std::set_intersection(aff_set.begin(), aff_set.end(), unique_nodes.begin(), unique_nodes.end(), std::inserter(result, result.begin()));
        float value;
        switch (variant)
        {
        case VARIANT_AFFUNIQUE_NORM:
            value = float(result.size()) / aff_set.size() + double(1)/edges.size();
            break;
        case VARIANT_AFFUNIQUE:
        default:
            value = result.size() + double(1)/edges.size();
            break;
        }
        
        affected.push_back(value);
        sum_affected += value;
    }

    // Get probabilities
    double prob;
    double sum = 0.0;
    probabilities.push_back(0.0);
    for(int i = 0; i < edges.size(); i++){
        prob = double(affected[i]) / sum_affected;
        probabilities.push_back(sum + prob);
        sum += prob;
    }

    double offset_start, offset_end;
    double rndNumber;
    // Roulette wheel selection for chosing edges
    while (selected_edges.size() < nr_edges){

        // Generate nr between 0.0 and 1.0
        rndNumber = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

        // Find chosen edge
        for (int j = 0; j < edges.size(); j++) {

            // Offset: start value edge + probability of chosing edge
            offset_start = probabilities[j];
            offset_end = probabilities[j + 1];
            // Edge found
            if (rndNumber >= offset_start && rndNumber < offset_end) {

                // Do not add double edges
                if(chosen.find(j) == chosen.end()){
                    chosen.insert(j);
                    selected_edges.push_back(edges[j]);
                }
                break;
            }
        }
    }
    return selected_edges;
}