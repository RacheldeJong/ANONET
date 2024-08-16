#include "./anonymization.h"

void Anonymization::anonymize(){
    int edges_altered = 0;
    int nr_todelete = recompute_gap;
    int its = ceil(float(budget) / recompute_gap);
    int nodes = g->get_number_nodes();
    clock_t time_start;
    time_start = clock();

    runtimes.push_back(((double)(clock() - (double)time_start))/CLOCKS_PER_SEC);
    uniqueness.push_back(EP->get_frac_k_anonymous_nodes(k-1, false));
    twin_uniqueness.push_back(EP->get_frac_k_anonymous_nodes(k-1, false));
    for(int i = 0; i < its; i++){
        // Delete only remaining edges
        if(edges_altered + recompute_gap > budget){
            nr_todelete = budget - edges_altered;
            if(nr_todelete == 0) break;
        }
        anonalg.update(g, EP);
        auto delete_edges = anonalg.select_edges(g, EP, nr_todelete);
        for(auto edge : delete_edges){
            g->delete_edge(edge.first, edge.second);
        }

        // Update EP and datastructures anonalg
        EP->update_ep();
        anonalg.update(g, EP);
        // Store statistics
        runtimes.push_back(((double)(clock() - (double)time_start))/CLOCKS_PER_SEC);
        uniqueness.push_back(EP->get_frac_k_anonymous_nodes(k-1, false));
        twin_uniqueness.push_back(EP->get_frac_k_anonymous_nodes(k-1, false));
        edges_altered += delete_edges.size();
        if(g->get_number_edges() == 0) break;
    }
}

void Anonymization::set_k(const int k_){
    k = k_;
}

void Anonymization::print_info(){
    std::cout<<"Budget: "<<budget<<std::endl;
    std::cout<<"Recompute gap: "<<recompute_gap<<std::endl;
    std::cout<<"Anonymization alg : ";
    if(anon_alg_nr == 1) std::cout<<"Unique"<<std::endl;
    else if(anon_alg_nr == 2) std::cout<<"Degree"<<std::endl;
    else if(anon_alg_nr == 3) std::cout<<"Affected nodes"<<std::endl;
    else if(anon_alg_nr == 4) std::cout<<"Affected unique nodes"<<std::endl;
    else std::cout<<"Random"<<std::endl;
}

void Anonymization::print_statistics(){
    std::cout<<"Altered, uniqueness, twin-uniqueness, runtime"<<std::endl;
    for(int i = 0; i < uniqueness.size(); i++){
        std::cout<<recompute_gap * i<<", ";
        std::cout<<uniqueness[i]<<", ";
        std::cout<<twin_uniqueness[i]<<", ";
        std::cout<<runtimes[i]<<", ";
        std::cout<<std::endl;
    }
}

void Anonymization::set_alg_variant(const int variant){
    anonalg.set_variant(variant);
}
