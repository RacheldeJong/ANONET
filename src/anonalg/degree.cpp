#include "degree.h"

std::vector< std::pair<int, int> > AnonDegree::select_edges(Graph *g, EquivalencePartition *EP, 
const int nr_edges){
    std::vector< std::pair<int, int> > unique_edges, selected_edges;
    auto edges = g->get_edges();
    std::vector<double> probabilities;

    // Return all edges left if nr_edges too large
    if(nr_edges >= edges.size()) return edges;

    // Get degrees and sum of degrees
    double degsum = 0.0;
    int deg1, deg2;
    double degres;
    std::vector<double> degresults;
    for(auto edge : edges){
        deg1 = g->get_degree(edge.first);
        deg2 = g->get_degree(edge.second);
        switch (variant)
        {
        case VARIANT_MAX:
            degres = std::max(deg1, deg2);
            break;
        case VARIANT_MIN:
            degres = std::min(deg1, deg2);
            break;
        case VARIANT_SUM:
            degres = deg1 + deg2;
            break;
        case VARIANT_DIFF:
            degres = abs(deg1 - deg2);
            break;
        default:
            degres = std::max(deg1, deg2);
            break;    
        }
        degres += double(1)/edges.size();
        degresults.push_back(degres);
        degsum += degres;
    }

    // Get probabilities
    double prob, prob_cur = 0.0;
    probabilities.push_back(0.0);
    for(auto degres : degresults){
        prob = float(degres) / degsum;
        prob_cur += prob;
        probabilities.push_back(prob_cur);
    }

    double rndNumber;
    double offset_start, offset_end;
    int choice = 0;
    std::set<int> chosen;

    // Iterate over edges, choose each with probability
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