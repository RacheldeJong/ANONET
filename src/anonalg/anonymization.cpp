#include "./anonymization.h"
#include "../equivalence/fuzzy.h"

std::set<int> Anonymization::compute_anonymity(float &uniqueness_val, const bool fuzzy){
    // Compute fuzzy-k-anonymity
    if(fuzzy && fuzzy_budget > 0){
        Ged ged = Ged(g, EP);
        FuzzyEP fuzzy = FuzzyEP(g, EP, &ged, fuzzy_budget, fuzzy_perc, false);
        uniqueness_val = fuzzy.get_frac_non_k_anonymous_nodes(k, use_twin_uniqueness);
        return fuzzy.get_non_k_anonymous_nodes(k, use_twin_uniqueness);
    }
    uniqueness_val = EP->get_frac_non_k_anonymous_nodes(k, use_twin_uniqueness);
    return EP->get_non_k_anonymous_nodes(k, use_twin_uniqueness);
}

void Anonymization::anonymize(){
    int nr_edges_altered = 0;
    int nr_todelete = recompute_gap;
    int its = ceil(float(budget) / recompute_gap);
    int nodes = g->get_number_nodes();
    int edges_start = g->get_number_edges();
    clock_t timer_start, timer_update, timer_alg;
    float time_total, time_update, time_alg, uniqueness_val, fuzzy_uniqueness_val;
    deleted_edges_all = {};

    // Insert initial values
    runtimes.push_back(0.0);
    runtimes_alg.push_back(0.0);
    runtimes_update.push_back(0.0);
    uniqueness.push_back(EP->get_frac_non_k_anonymous_nodes(k, false));
    twin_uniqueness.push_back(EP->get_frac_non_k_anonymous_nodes(k, true));
    edges_altered.push_back(0);
    perc_edges_altered.push_back(0.0);
    std::set<int> non_anon_nodes, fuzzy_non_anon_nodes;
    non_anon_nodes = compute_anonymity(uniqueness_val, false);
    if(fuzzy_budget>0){
        fuzzy_non_anon_nodes = compute_anonymity(fuzzy_uniqueness_val, true);
        if(use_fuzzy)non_anon_nodes = fuzzy_non_anon_nodes;
        fuzzy_uniqueness.push_back(fuzzy_uniqueness_val);
    }

    for(int i = 0; i < its; i++){
        timer_start = clock();

        // Delete only remaining edges
        if(nr_edges_altered + recompute_gap > budget){
            nr_todelete = budget - nr_edges_altered;
            if(nr_todelete == 0) break;
        }
        if(non_anon_nodes.size() == 0 || (fuzzy_budget > 0 && fuzzy_non_anon_nodes.size() == 0)){
            runtimes_alg.push_back(0.0);
            runtimes_update.push_back(0.0);
            runtimes.push_back(0.0);
            break;
        }
        timer_alg = clock();
        auto delete_edges = anonalg.select_edges(g, EP, non_anon_nodes, nr_todelete);
        time_alg += ((double)(clock() - (double)timer_alg))/CLOCKS_PER_SEC;
        runtimes_alg.push_back(time_alg);

        timer_update = clock();
        for(auto edge : delete_edges){
            g->delete_edge(edge.first, edge.second);
            deleted_edges_all.push_back(edge);
        }
        // Update EP and datastructures anonalg
        EP->update_ep();
        time_update += ((double)(clock() - (double)timer_update))/CLOCKS_PER_SEC;
        runtimes_update.push_back(time_update);

        // Store statistics
        time_total += ((double)(clock() - (double)timer_start))/CLOCKS_PER_SEC;
        runtimes.push_back(time_total);
        
        // Compute anonymity
        non_anon_nodes = compute_anonymity(uniqueness_val, false);
        if(fuzzy_budget>0){
            fuzzy_non_anon_nodes = compute_anonymity(fuzzy_uniqueness_val, true);
            fuzzy_uniqueness.push_back(fuzzy_uniqueness_val);
            if(use_fuzzy) non_anon_nodes = fuzzy_non_anon_nodes;
        }
        uniqueness.push_back(EP->get_frac_non_k_anonymous_nodes(k, false));
        twin_uniqueness.push_back(EP->get_frac_non_k_anonymous_nodes(k, true));

        nr_edges_altered += delete_edges.size();
        edges_altered.push_back(nr_edges_altered);
        perc_edges_altered.push_back((double)nr_edges_altered / edges_start);
        if(g->get_number_edges() == 0) break;
    }
}

void Anonymization::set_k(const int k_){
    k = k_;
}

void Anonymization::set_twin_uniqueness(const bool twin_uniqueness){
    use_twin_uniqueness = twin_uniqueness;
}

void Anonymization::set_fuzzy_anon(const int fuzzy_budget_, const bool fuzzy_perc_, const bool use_fuzzy_){
    fuzzy_budget = fuzzy_budget_;
    fuzzy_perc = fuzzy_perc_;
    use_fuzzy = use_fuzzy_;
}

const void Anonymization::print_info(){
    std::cout<<"Budget: "<<budget<<std::endl;
    std::cout<<"Recompute gap: "<<recompute_gap<<std::endl;
    std::cout<<"Anonymization alg : ";
    if(anon_alg_nr == 1) std::cout<<"Unique"<<std::endl;
    else if(anon_alg_nr == 2) std::cout<<"Degree"<<std::endl;
    else if(anon_alg_nr == 3) std::cout<<"Affected nodes"<<std::endl;
    else if(anon_alg_nr == 4) std::cout<<"Affected unique nodes"<<std::endl;
    else std::cout<<"Random"<<std::endl;
    std::cout<<"Fuzzy budget: "<<fuzzy_budget<<std::endl;
    std::cout<<"Fuzzy perc: "<<fuzzy_perc<<std::endl;
    std::cout<<"Fuzzy alg: "<<use_fuzzy<<std::endl;
}

const void Anonymization::print_statistics(){
    std::cout<<"Altered, uniqueness, twin-uniqueness, fuzzy-uniqueness, runtime, runtime-alg, runtime-update, edges_altered, perc_edges_altered"<<std::endl;
    for(int i = 0; i < uniqueness.size(); i++){
        std::cout<<recompute_gap * i<<", ";
        std::cout<<uniqueness[i]<<", ";
        std::cout<<twin_uniqueness[i]<<", ";
        if(fuzzy_budget>0) std::cout<<fuzzy_uniqueness[i]<<", ";
        else std::cout<<uniqueness[i]<<", ";
        std::cout<<runtimes[i]<<", ";
        std::cout<<runtimes_alg[i]<<", ";
        std::cout<<runtimes_update[i]<<", ";
        std::cout<<edges_altered[i]<<", ";
        std::cout<<perc_edges_altered[i]<<", ";
        std::cout<<std::endl;
    }
}

const void Anonymization::print_altered_edges(){
    std::cout<<"Altered edges. Edge a, b, operation"<<std::endl;
    for(auto edge : deleted_edges_all){
        std::cout<<"Alter, "<<edge.first<<", "<<edge.second<<", "<<OP_DELETE<<std::endl;
    }
}

void Anonymization::set_alg_variant(const int variant){
    anonalg.set_variant(variant);
}
