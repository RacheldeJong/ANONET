#include "equivalence.h"

std::vector< std::set<int> > EquivalencePartition::split_eclass(std::set<int> eclass){
    std::vector< std::set<int> > ec_split;
    std::map<std::pair<int, std::map<int, int>>, int>::iterator it;
    std::map<cache_pair, int> val_to_eq = {};
    cache_pair val;
    int ec_class;

    // No comparisons required
    if(eclass.size() == 1) {
        return {eclass};
    }

    // Find eq for each node in current eq
    for(auto node1 : eclass){
        val = cache.get_cache_entry(node1);
        // Comp: log(n) in nr. equivalence classes
        it = val_to_eq.find(val);
        if(it != val_to_eq.end()){
            ec_class = it->second;
            ec_split[ec_class].insert(node1);
        }
        // No equivalence class found. Create new
        else{
            val_to_eq[val] = ec_split.size();
            ec_split.push_back({node1});
        }
    }
    return ec_split;
}

std::vector< std::set<int> > EquivalencePartition::get_heuristic_partition(std::vector< std::set<int> > eq_cur, const int cur_dist){
    // Heuristic EP already initialized
    if(heuristicEPS.size() >= cur_dist) 
        return heuristicEPS[cur_dist-1]->get_equivalence_partition();

    // Compute partition
    EquivalencePartition *ep_heur;
    if(max_dist == 1) ep_heur = new EquivalencePartition(g, eq_cur, heur_choice, heur_choice);
    else ep_heur = new EquivalencePartition(g, eq_cur, std::make_pair(heur_choice, cur_dist), heur_choice);

    // Store and return
    heuristicEPS.push_back(ep_heur);
    return ep_heur->get_equivalence_partition();
}

std::vector< std::set <int > > EquivalencePartition::get_distance_partition(std::vector< std::set<int> > eq_cur, const int cur_dist){
    // EP already initialized
    if(distEPs.size() >= cur_dist)
        return distEPs[cur_dist-1]->get_equivalence_partition();
    
    // Compute partition
    EquivalencePartition *ep = new EquivalencePartition(g, eq_cur, std::make_pair(meas_choice, cur_dist), meas_choice);

    // Store and return partition
    distEPs.push_back(ep);
    return ep->get_equivalence_partition();
}

EquivalencePartition* EquivalencePartition::get_heuristic_partition(const int cur_dist){
    EquivalencePartition* ep;
    try{
        if(heuristicEPS.size() < cur_dist) throw(cur_dist);
        ep = heuristicEPS[cur_dist-1];
    }
    catch(const int dist){
        std::cerr<<"Error: heuristic EP for distance "<<dist<<" does not exist."<<std::endl;
    }   
    return ep;
}

EquivalencePartition* EquivalencePartition::get_distance_partition(const int cur_dist){
    EquivalencePartition* ep;
    try{
        if(distEPs.size() < cur_dist) throw(cur_dist);
        ep = distEPs[cur_dist-1];
    }
    catch(const int dist){
        std::cerr<<"Error: EP for distance "<<dist<<" does not exist."<<std::endl;
    }   
    return ep;
}

void EquivalencePartition::compute_equivalence_partition(){
    std::vector< std::set< int > >ep_cur, ep_new;
    std::vector<int> minimal_nodes;
    std::vector<int> node_set;
    size_t i;
    int it_nr = 1;

    clock_t time_it, time_tot;
    // Start timer total time
    time_tot = clock();

    // Initialize eq
    if(ep_initial.size() == 0) ep_cur = {g->get_nodes()};
    else ep_cur = ep_initial;

    g->init_twin();
    ep_cur = remove_twins(ep_cur);

    for(int cur_dist = 1; cur_dist <= max_dist; cur_dist++){
        // Start timer current iteration
        time_it = clock();
        if(!iterative_computation) cur_dist = max_dist;

        // Iterative approach: compute previous values
        if(cur_dist != max_dist){
            ep_cur = get_heuristic_partition(ep_cur, cur_dist);
            ep_cur = get_distance_partition(ep_cur, cur_dist);
            runtimes.push_back(((double)(clock() - (double)time_it))/CLOCKS_PER_SEC);
            continue;
        }

        // Compute for current level
        if(heur_choice != meas_choice){
            ep_cur = get_heuristic_partition(ep_cur, cur_dist);
        }

        // Compute equivalence classes by splitting each eq
        for(auto eq : ep_cur){

            for(auto it2 : split_eclass(eq)){
                ep_new.push_back(it2);
            }
        }
        runtimes.push_back(((double)(clock() - (double)time_it))/CLOCKS_PER_SEC);
    }
    fflush(stdout);

    ep_new = add_twins(ep_new);

    // Store computed equivalence class
    epartition = ep_new;
    compute_node_to_ec();

    // Store total runtime
    runtime_tot = ((double)(clock() - (double)time_tot))/CLOCKS_PER_SEC;
}

std::vector< std::set<int>> EquivalencePartition::remove_twins(std::vector<std::set<int>> ep_cur){
    auto node_to_twin = g->twinnodes->get_node_to_twin();
    auto minimal_twins_vec = g->twinnodes->get_minimal_twins();
    std::set<int> minimal_twins_set;
    std::vector< std::set<int>> ep_new;
    std::set<int> eq_new;

    for(auto node : minimal_twins_vec) minimal_twins_set.insert(node);

    for(auto eq : ep_cur){
        eq_new = {};
        for(auto node : eq){
            if(minimal_twins_set.find(node) != minimal_twins_set.end()){
                eq_new.insert(node);
            }
        }
        ep_new.push_back(eq_new);
    }
    return ep_new;
}

std::vector< std::set<int>> EquivalencePartition::add_twins(std::vector<std::set<int>> ep_cur){
    auto node_to_twin = g->twinnodes->get_node_to_twin();
    auto twin_nodes = g->twinnodes->get_twinnodes();
    int twin_class;
    std::vector< std::set<int>> ep_new;
    std::set<int> eq_new;

    for(auto eq : ep_cur){
        eq_new = {};
        for(auto node : eq){
            twin_class = node_to_twin[node];
            for(auto twin : twin_nodes[twin_class]){
                eq_new.insert(twin);
            }
        }
        ep_new.push_back(eq_new);
    }
    return ep_new;
}

void EquivalencePartition::compute_node_to_ec(){
    node_to_ep_entry = std::vector<int> (g->get_number_nodes(), -1);
    int eq_id, node;

    eq_id = 0;
    for(auto eq : epartition){
        for(auto node : eq){
            node_to_ep_entry[node] = eq_id;
        }
        eq_id++;
    }
}

std::set<int> EquivalencePartition::update_node(const int node1, std::set<int> ec_initial){
    cache_pair val1, val2;
    int ep_entry;
    std::set<int> ec_cur, ec_notcomputed;
    
    cache.update_cache_entry(node1);
    val1 = cache.get_cache_entry(node1);
    ec_cur = ec_initial;

    // Iterate twice: first loop, only use computed values, second loop
    // compute values for nodes for which it was not yet computed
    for(int i = 0; i < 2; i++){
        for(auto node2 : ec_cur){
            if(node1 == node2 || node_to_ep_entry[node2] == -1){
                continue;
            }
            // Get value
            if(i == 0) val2 = cache.get_cache_entry_computed(node2);
            else if(i == 1) val2 = cache.get_cache_entry(node2);

            // If value not computed, compute in second round
            if(val2 == EMPTY_CACHE_PAIR){
                ec_notcomputed.insert(node2);
                continue;
            }

            // Fount correct ec, insert to EP and update
            if(val1 == val2){
                ep_entry = node_to_ep_entry[node2];
                epartition[ep_entry].insert(node1);
                node_to_ep_entry[node1] = ep_entry;
                return epartition[ep_entry];
            }
        }
        ec_cur = ec_notcomputed;
    }
    // No EC found for node1
    ep_entry = epartition.size();
    epartition.push_back({node1});
    node_to_ep_entry[node1] = ep_entry;
    epartition[ep_entry];
    return epartition[ep_entry];
}

void EquivalencePartition::remove_nodes_from_ep(std::set<int> nodes){
    int ep_entry;
    for(auto node : nodes){
        ep_entry = node_to_ep_entry[node];
        epartition[ep_entry].erase(node);
        node_to_ep_entry[node] = -1;
    }
}

void EquivalencePartition::remove_empty_ecs(){
    std::vector<std::set<int>>::iterator ep_it;
    std::vector<int> diff;
    int diff_cur = 0;
    int size;

    for(ep_it = epartition.begin(); ep_it < epartition.end();){
        size = ep_it->size();
        // Empty EC: remove
        if(size == 0){
            diff_cur += 1;
            epartition.erase(ep_it);
            continue;
        }
        // Update not_to_ep_entry for remaining ecs
        for(auto node : *ep_it){
            node_to_ep_entry[node] -= diff_cur;
        }
        ep_it++;
    }
}

void EquivalencePartition::update_nodes(std::set<int> nodes){
    // Remove nodes from EP
    remove_nodes_from_ep(nodes);
    for(int d_cur = 1; d_cur<max_dist; d_cur++){
        heuristicEPS[d_cur-1]->remove_nodes_from_ep(nodes);
        distEPs[d_cur-1]->remove_nodes_from_ep(nodes);
    }

    std::set<int> all_nodes = g->get_nodes();
    std::set<int> ec_cur;
    // Find new position in EP
    for(auto node : nodes){
        ec_cur = all_nodes;
        // Update for previous heuristics and previous distances
        for(int d_cur = 1; d_cur<max_dist; d_cur++){
            if(heuristicEPS.size() >= d_cur) 
                ec_cur = heuristicEPS[d_cur-1]->update_node(node, ec_cur);
            if(!iterative_computation) 
                break;
            if (distEPs.size() >= d_cur)
                ec_cur = distEPs[d_cur-1]->update_node(node, ec_cur);
        }
        // Update for node in current EP
        update_node(node, ec_cur);
    }
}

void EquivalencePartition::update_ep(){
    auto updated_edges = g->get_updated_edges();
    std::set<int> nodes = {};
    std::set<int> new_nodes = {};
    std::pair<int, int> edge;

    // Get nodes that should be updated
    for(int i = update_count; i<updated_edges.size(); i++){
        edge = updated_edges[i].first;
        new_nodes = cache.eq_measure.get_nodes_affected(g, edge.first, edge.second);
        for(auto node : new_nodes){
            nodes.insert(node);
        }
    }

    update_nodes(nodes);
    update_count = updated_edges.size();
    remove_empty_ecs();
}

std::vector< std::set<int> > EquivalencePartition::get_equivalence_partition(){
    if(epartition.size() == 0)
        compute_equivalence_partition();
    return epartition;
}

const std::vector<int> EquivalencePartition::get_representative_nodes(){
    std::vector<int> node_set;
    if(epartition.size() == 0)
        compute_equivalence_partition();
    
    for(auto it : epartition){
        node_set.push_back(*it.begin());
    }
    return node_set;
}

const std::vector<int> EquivalencePartition::get_node_to_ep_entry(){
    return node_to_ep_entry;
}

int EquivalencePartition::get_eq_entry(const int v){
    if(node_to_ep_entry.size() == 0) compute_node_to_ec();
    return node_to_ep_entry[v];
}

void EquivalencePartition::set_twin_heuristic(const bool value){
    twin_heuristic = value;
}

void EquivalencePartition::set_iterative(const bool value){
    iterative_computation = value;
}

const bool EquivalencePartition::get_twin_heuristic(){
    return twin_heuristic;
}

const bool EquivalencePartition::get_iterative(){
    return iterative_computation;
}

const void EquivalencePartition::print_info(){
    std::cout<<"Equivalence partition info: "<<std::endl;
    
    // Measure used
    std::cout<<"\tUsed measure: ";
    if(meas_choice == MEAS_DEGREE) std::cout<<"Degree"<<std::endl;
    else if(meas_choice == MEAS_DEGDIST) std::cout<<"Degree distribution"<<std::endl;
    else if(meas_choice == MEAS_COUNT) std::cout<<"Count"<<std::endl;
    else if(meas_choice == MEAS_HAY) std::cout<<"Hay"<<std::endl;
    else if(meas_choice == MEAS_DKANON) std::cout<<"d-k-Anonymity"<<std::endl;
    else if(meas_choice == MEAS_HYBRID) std::cout<<"Hybrid"<<std::endl;
    else std::cout<<"none"<<std::endl;

    // Maximum distance
    std::cout<<"\tMaximum distance: "<<max_dist<<std::endl;

    // Heuristic used
    std::cout<<"\tHeuristic used: ";
    if(heur_choice == MEAS_DEGREE) std::cout<<"Degree"<<std::endl;
    else if(heur_choice == MEAS_DEGDIST) std::cout<<"Degree distribution"<<std::endl;
    else if(heur_choice == MEAS_COUNT) std::cout<<"Count"<<std::endl;
    else if(heur_choice == MEAS_HAY) std::cout<<"Hay"<<std::endl;
    else if(heur_choice == MEAS_DKANON) std::cout<<"d-k-Anonymity"<<std::endl;
    else if(heur_choice == MEAS_HYBRID) std::cout<<"Hybrid"<<std::endl;
    else std::cout<<"none"<<std::endl;

    // Twin nodes used
    std::cout<<"\tTwin nodes used: ";
    if(twin_heuristic == true) std::cout<<"true"<<std::endl;
    else std::cout<<"false"<<std::endl;

    // Computed
    std::cout<<"\tEquivalence partition computed: ";
    if(epartition.size() > 0) std::cout<<"true"<<std::endl;
    else std::cout<<"false"<<std::endl;
}

const std::map<int, int> EquivalencePartition::get_equivalence_distribution(std::vector<std::set<int>>epartition_cur){
    std::map<int, int> size_dict;
    int size;

    for(auto it : epartition_cur){
        size = it.size();
        if(size_dict.find(size) != size_dict.end()){
            size_dict[size] ++;
        }
        else{
            size_dict[size] = 1;
        }
    }
    return size_dict;
}

const std::map<int, int> EquivalencePartition::get_equivalence_distribution(){
    return get_equivalence_distribution(epartition);
}

const std::vector<int> EquivalencePartition::get_node_to_anonymity(bool minustwin){
    auto epartition_cur = epartition;
    int entry, size;
    std::vector<int> node_to_anonymity;

    if(minustwin){
        epartition_cur = remove_twins(epartition_cur);
    }

    for(int node_id = 0; node_id < node_to_ep_entry.size(); node_id++){
        entry = node_to_ep_entry[node_id];
        size = epartition_cur[entry].size();
        node_to_anonymity.push_back(size);
    }
    return node_to_anonymity;
}

const void EquivalencePartition::print_equivalence_partition(){
    auto epartition_cur = epartition;
    
    for(auto eclass : epartition_cur){
        for(auto node :eclass){
            std::cout<<node<<", ";
        }
        std::cout<<std::endl;
    }
}

const void EquivalencePartition::print_equivalence_distribution(bool minustwin){
    auto epartition_cur = epartition;
    if(minustwin){
        epartition_cur = remove_twins(epartition_cur);
    }
    
    std::map<int, int> size_dict = get_equivalence_distribution(epartition_cur);

    for(auto it : size_dict){
        std::cout<<"["<<it.first<<", "<<it.second<<"], ";
    }
    std::cout<<std::endl;
}

const void EquivalencePartition::print_node_anonymity(bool minustwin){
    auto epartition_cur = epartition;
    int entry, size;

    if(minustwin){
        epartition_cur = remove_twins(epartition_cur);
    }

    std::cout<<node_to_ep_entry.size()<<std::endl;
    std::cout<<epartition_cur.size()<<std::endl;
    for(int node_id = 0; node_id < node_to_ep_entry.size(); node_id++){
        entry = node_to_ep_entry[node_id];
        size = epartition_cur[entry].size();
        std::cout<<node_id<<" -> "<<size<<std::endl;
    }
}

const std::set<int> EquivalencePartition::get_k_anonymous_nodes(const int k, const bool minustwin){
    auto epartition_cur = epartition;
    std::set<int> k_nodes;
    std::map<int, int> anon_distr = get_anonymity_distribution(minustwin);
    int k_node, ep_entry;

    if(minustwin){
        epartition_cur = remove_twins(epartition_cur);
    }

    for(int node_id = 0; node_id < node_to_ep_entry.size(); node_id++){
        ep_entry = node_to_ep_entry[node_id];
        k_node = epartition_cur[ep_entry].size();
        if(k_node <= k){
            k_nodes.insert(node_id);
        }
    }

    return k_nodes;
}

const double EquivalencePartition::get_frac_k_anonymous_nodes(const int k, const bool minustwin){
    auto nodes = get_k_anonymous_nodes(k, minustwin);
    int nr_nodes = g->get_number_nodes();
    return double(nodes.size()) / nr_nodes;
}

const std::map<int, int> EquivalencePartition::get_anonymity_distribution(bool minustwin){
    auto epartition_cur = epartition;
    int ep_entry, size;
    std::map<int, int> anon_distr = {};

    if(minustwin){
        epartition_cur = remove_twins(epartition_cur);
    }

    for(int node_id = 0; node_id < node_to_ep_entry.size(); node_id++){
        ep_entry = node_to_ep_entry[node_id];
        size = epartition_cur[ep_entry].size();
        if(anon_distr.find(size) == anon_distr.end()){
            anon_distr[size] = 1;
        }
        else{
            anon_distr[size]++;
        }
    }
    return anon_distr;
}

const void EquivalencePartition::print_anonymity_distribution(bool minustwin){
    std::map<int, int> anon_dist = get_anonymity_distribution(minustwin);
    int unique = 0;
    for(auto it : anon_dist){
        std::cout<<"["<<it.first<<", "<<it.second<<"], ";
        if(it.first == 1){
            unique += it.second;
        }
    }
    std::cout<<std::endl;
    std::cout<<"Fraction unique: "<<double(unique) / g->get_number_nodes()<<std::endl;
}

const void EquivalencePartition::print_runtimes(){
    int cur_dist = 1;
    for(auto runtime : runtimes){
        std::cout<<"Runtime d="<<cur_dist<<": "<<runtime<<std::endl;
        cur_dist++;
    }
    std::cout<<"Runtime total: "<<runtime_tot<<std::endl;
}
