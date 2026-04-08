#include "ged.h"

void Ged::compute_ged_matrix(){
    Measure eq_measure = ep->cache.eq_measure;
    auto nodes = ep->get_representative_nodes();
    std::vector<int> row;
    int difference;
    ged_matrix = {};

    std::map<int, cache_pair> values;
    for(auto node : nodes){
        values[node] = ep->cache.get_cache_entry(node);
    }
    
    // Ged matrix
    clock_t time_ged = clock();
    for(auto node1 : nodes){
        row = {};
        // Compute edit value node1
        for(auto node2 : nodes){
            // Get or compute edit value node2
            difference = eq_measure.compute_difference(g, values[node1], values[node2]);
            row.push_back(difference);
        }
        ged_matrix.push_back(row);
    }
    // Store time
    double runtime = (double)(clock() - (double)time_ged)/CLOCKS_PER_SEC;
}

const int Ged::get_ged_value(const int i, const int j){
    if(i > ged_matrix.size() || j > ged_matrix[i].size()){
        return -1;
    }
    return ged_matrix[i][j];
}

const void Ged::print_ged_matrix(){
    for(auto row : ged_matrix){
        for(auto dist : row){
            std::cout<<dist<<", ";
        }
        std::cout<<std::endl;
    }
}

const std::vector<std::vector<int>> Ged::get_ged_matrix(){
    return ged_matrix;
}