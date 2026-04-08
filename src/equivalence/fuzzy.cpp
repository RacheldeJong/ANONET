#include "fuzzy.h"

// Output: fuzzy_ep
// Maps each node to nodes to which it is fuzzy-equivalent
// For each node, n fuzzy_ep[n] -> [a, b, c, d, ...]
// So that distance(n, a) <= max_diff
void FuzzyEP::compute_fuzzy_ep(const int max_diff, const bool perc, const bool use_twins){
    auto nodes = ep->get_representative_nodes();
    std::vector<int> row;
    int difference, node1, node2, k_node1, k_node2, k_fuzzy, ep_entry, val_int;
    std::map<int, int> anon_dist;
    float diff_perc, max_diff_perc = 0.0;
    cache_pair val;

    // Normalize if allowing for a percentage of difference
    if(perc){
        max_diff_perc = float(max_diff) / float(100);
    }

    // Get k-values EPs
    auto node_to_anon = ep->get_node_to_anonymity(use_twins);
    auto node_to_occ = ep->get_node_to_anonymity(false);
    auto partition = ep->get_equivalence_partition();

    // Ged matrix
    clock_t t1 = clock();
    for(int i = 0; i < nodes.size(); i++){
        // Get representative node for ep.
        // Each iteration: compute fuzzy k-value for node1
        node1 = nodes[i];
        // Add original class
        fuzzy_ep[node1]  = {};
        ep_entry = ep->get_eq_entry(node1);
        for(auto node_add : partition[ep_entry]){
            fuzzy_ep[node1].insert(node_add);
        }

        // Compute distance with all other eqs to determine phi-equivalence
        for(int j = 0; j < nodes.size(); j++){
            if(i == j) continue;
            node2 = nodes[j];
            difference = ged->get_ged_value(i, j);
            // Compute values perc.
            if(perc){
                val = ep->cache.get_cache_entry(node1);
                val_int = ep->cache.get_cache_entry_int_value(node1);
                diff_perc = (float)difference / val_int;
            }
            // Test for fuzzy equivalence: exact difference, or perc. difference
            if((! perc && difference <= max_diff) || 
            (perc && diff_perc <= (float)max_diff_perc )){
                // Equivalent: insert node in same eq as node2
                ep_entry = ep->get_eq_entry(node2);
                for(auto node_add : partition[ep_entry]){
                    fuzzy_ep[node1].insert(node_add);
                }
            }
        }
    }
    runtime = ((double)(clock() - (double)t1))/CLOCKS_PER_SEC;
}

void FuzzyEP::compute_fuzzy_anon_dist(){
    int ep_entry, ep_size, fuzzy_ep_size;
    auto partition =  ep->get_equivalence_partition();
    fuzzy_anon_dist = {};

    for(auto node : fuzzy_ep){
        ep_entry = ep->get_eq_entry(node.first);
        ep_size = partition[ep_entry].size();
        fuzzy_ep_size = node.second.size();
        // Fuzzy k-anonymity size occurs for ep_size nodes
        if(fuzzy_anon_dist.find(fuzzy_ep_size) == fuzzy_anon_dist.end()){
            fuzzy_anon_dist[fuzzy_ep_size] = ep_size;
        }
        else{
            fuzzy_anon_dist[fuzzy_ep_size] += ep_size;
        }
    }
}

const std::map<int, std::set<int>> FuzzyEP::get_fuzzy_ep(){
    return fuzzy_ep;
}

const std::map<int, int> FuzzyEP::get_fuzzy_anonymity_distribution(){
    return fuzzy_anon_dist;
}

const void FuzzyEP::print_node_to_equivalent(){
    // Print node: equivalent nodes,
    for(auto node1 : fuzzy_ep){
        for(auto node_considered : node1.second){
            std::cout<<node_considered<<": ";
            for(auto node_similar : node1.second){
                if(node_considered != node_similar)
                    std::cout<<node_similar<<", ";
            }
            std::cout<<std::endl;
        }
    }
}

const void FuzzyEP::print_fuzzy_distribution(){
    // [size, nr_nodes for with this value]
    for(auto it : fuzzy_anon_dist){
        std::cout<<"["<<it.first<<","<<it.second<<"],";
    }
    std::cout<<std::endl;
}

const void FuzzyEP::print_statistics(){
    std::cout<<"Runtime: "<<runtime<<std::endl;
}

const std::set<int> FuzzyEP::get_non_k_anonymous_nodes(const int k, const bool use_twin_uniqueness){
    std::map<int, int> fuzzy_anon_dist = get_fuzzy_anonymity_distribution();
    int k_node, k_anonymous_nodes, nr_nodes;
    std::set<int> non_anon_nodes = {};
    fuzzy_ep; // Contains for each node to which nodes it is anonymous
    for(auto it : fuzzy_ep){
        // k-anonymous node found: do not add
        if(it.second.size()<k) non_anon_nodes.insert(it.first);
    }
    return non_anon_nodes;
}

const double FuzzyEP::get_frac_non_k_anonymous_nodes(const int k, const bool use_twin_uniqueness){
    int nr_nodes_total = g->get_number_nodes();
    std::map<int, int> fuzzy_anon_dist = get_fuzzy_anonymity_distribution();
    int k_node, k_anonymous_nodes, nr_nodes;
    k_anonymous_nodes = 0;
    int tot_nodes = 0;
    for(auto it : fuzzy_anon_dist){
        k_node = it.first;
        nr_nodes = it.second;
        if(k_node < k){
            k_anonymous_nodes += nr_nodes;
        }
    }
    return double(k_anonymous_nodes) / nr_nodes_total;
}

const void FuzzyEP::print_info(){
    ep->print_info();
    std::cout<<"Fuzzy similarity info: "<<std::endl;
    std::cout<<"\tPhi value: "<<max_diff<<"%"<<std::endl;

}