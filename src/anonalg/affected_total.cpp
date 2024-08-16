#include "affected_total.h"

std::vector< std::pair<int, int> > AnonAffectedTotal::select_edges(Graph *g, EquivalencePartition *EP, 
const int nr_edges){
    std::vector< std::pair<int, int> > selected_edges;
    auto edges = g->get_edges();
    std::vector<int> affected;
    std::vector<double> probabilities;
    Measure m = EP->cache.eq_measure;

    // Return all edges left if nr_edges too large
    if(nr_edges >= edges.size()) return edges;

    // Get degrees and sum of degrees
    double nr_affected;
    double sum_affected = 0;
    for(auto edge : edges){
        nr_affected = (m.get_nodes_affected(g, edge.first, edge.second)).size();
        affected.push_back(nr_affected + double(1)/edges.size());
        sum_affected += nr_affected;
    }

    // Get probability ranges
    double prob;
    double sum = 0.0;
    probabilities.push_back(0.0);
    for(int i = 0; i < edges.size(); i++){
        prob = double(affected[i]) / sum_affected; // Make sure every edge can be chosen
        probabilities.push_back(sum + prob);
        sum += prob;
    }

    double rndNumber;
    double offset_start, offset_end;
    std::set<int> chosen;
    // Roulette wheel selection for chosing edges
    while (selected_edges.size() < nr_edges){

        // Make sure nr. between 0.0 and 1.0
        rndNumber = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

        // Find chosen edge
        for (int j = 0; j < edges.size()-1; j++) {
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