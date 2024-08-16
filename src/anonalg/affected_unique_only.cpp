#include "affected_unique_only.h"

std::vector< std::pair<int, int> > AnonAffectedUniqueOnly::select_edges(Graph *g, EquivalencePartition *EP, 
const int nr_edges){
    std::vector< std::pair<int, int> > unique_edges, selected_edges;
    auto edges = g->get_edges();
    std::vector<double> affected;
    std::vector<double> probabilities;
    Measure m = EP->cache.eq_measure;

    // Return all edges left if nr_edges too large
    if(nr_edges >= edges.size()) return edges;

    auto unique_nodes = EP->get_k_anonymous_nodes(k-1, false);

    // Find edges that are connected to at least one unique node
    for(auto edge : edges){
        switch (variant)
        {
            // Select all edges connecting two unique nodes
            case VARIANT_AFFUNIQUE_UU:     
            case VARIANT_AFFUNIQUE_UU_NORM:
                if (unique_nodes.find(edge.first) != unique_nodes.end() &&
                unique_nodes.find(edge.second) != unique_nodes.end()){
                    unique_edges.push_back(edge);
                }
                break;
            // Select all edges connecting at least one unique node
            case VARIANT_AFFUNIQUE_U:
            case VARIANT_AFFUNIQUE_U_NORM:
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
            if(selected_edges.size() >= nr_edges) break;
            int elements_before = selected_edges.size();
            chosen_edges.insert(edge);
            if(elements_before == chosen_edges.size()) continue;
            selected_edges.push_back(edge);
        }
        return selected_edges;
    }
    // Get degrees and sum of degrees
    int nr_affected;
    double sum_affected = 0;
    std::set<int> result, aff_set;
    std::set<int> chosen;

    for(auto edge : unique_edges){
        result = {};
        aff_set = m.get_nodes_affected(g, edge.first, edge.second);
        // Overlap unique nodes, aff_set
        std::set_intersection(aff_set.begin(), aff_set.end(), unique_nodes.begin(), unique_nodes.end(), std::inserter(result, result.begin()));
        float value;
        switch (variant)
        {
        case VARIANT_AFFUNIQUE_U:
        case VARIANT_AFFUNIQUE_UU:
            value = result.size() + double(1)/edges.size();
            break;
        case VARIANT_AFFUNIQUE_U_NORM:
        case VARIANT_AFFUNIQUE_UU_NORM:
        default:
            value = float(result.size()) / aff_set.size()+ double(1)/edges.size();
            break;
        }
        affected.push_back(result.size());
        sum_affected += result.size();
    }

    // Get probabilities
    double prob;
    double sum = 0.0;
    probabilities.push_back(0.0);
    for(int i = 0; i < unique_edges.size(); i++){
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
        for (int j = 0; j < unique_edges.size()-1; j++) {
            // Offset: start value edge + probability of chosing edge
            offset_start = probabilities[j];
            offset_end = probabilities[j + 1];
            // Edge found
            if (rndNumber >= offset_start && rndNumber < offset_end) {
                // Do not add double edges
                if(chosen.find(j) == chosen.end()){
                    chosen.insert(j);
                    selected_edges.push_back(unique_edges[j]);
                }
                break;
            }
        }
    }
    return selected_edges;
}