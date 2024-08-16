#include "cascade.h"

std::map<int, std::set<int> > AnonymityCascade::get_eq_dist_nodeset(Graph *g, std::vector<int> node_to_ep, std::set<int> nodes){
    std::map<int, std::set<int> > eq_dist;
    int ep_id;

    for(auto node : nodes){
        ep_id = node_to_ep[node];
        if(eq_dist.find(ep_id) != eq_dist.end()){
            eq_dist[ep_id].insert(node);
        }
        else{
            eq_dist[ep_id] = {node};
        }
    }
    return eq_dist;
}

/*
    Nodes are unique in eq if:
    - It is in an equivalence class of size 1
    - All nodes in the equivalence class are twins of each other
*/
std::set<int> AnonymityCascade::get_unique_nodes(Graph *g, std::map< int, std::set<int> > eq, bool twinnodes){
    std::vector<int> node_to_twin;
    std::set<int> unique_nodes;
    int twin_id;
    bool unique;

    if(twinnodes){
        node_to_twin = g->twinnodes->get_node_to_twin();
    }

    for(auto it : eq){
        unique = false;
        // Check if nodes are unique or twin-unique
        if(it.second.size() == 1)
            unique = true;
        else if(twinnodes){
            unique = true;
            twin_id = node_to_twin[*it.second.begin()];
            for(auto node : it.second){
                if(twin_id != node_to_twin[node]){
                    unique = false;
                    break;
                }
            }
        }
        // Add unique nodes
        if(unique)
            for(auto node : it.second)
                unique_nodes.insert(node);
    }
    return unique_nodes;
}

std::set<int> AnonymityCascade::anonymity_cascade(Graph *g, EquivalencePartition *EP1, EquivalencePartition *EP2, bool twinnodes, const int level){
    auto node_to_ep2 = EP2->get_node_to_ep_entry();
    std::map< int, std::set<int> > eq_dist;
    std::vector<int> unique_check(g->get_number_nodes(), 0);
    int cur_level;
    unique_level = {};

    // Find unique or twin-unique nodes
    std::set<int> unique_all;
    std::queue<int> unique_it, unique_new;

    // Start timer
    clock_t time_tot;
    time_tot = clock();

    // Get unique nodes
    unique_all = EP1->get_k_anonymous_nodes(K_UNIQUE, twinnodes);
    for(auto node : unique_all){
        unique_it.push(node);
        unique_check[node] = 1;
    }
    unique_level.push_back(unique_all.size());

    // Cascading process: iterate over unique nodes
    while(!unique_it.empty()){
        int node = unique_it.front();
        unique_it.pop();
        auto nbs = g->get_neighbors(node);

        // Get equivalence distribution over neighbors
        auto eq_dist_nbs = get_eq_dist_nodeset(g, node_to_ep2, nbs);
        auto unique_nodes = get_unique_nodes(g, eq_dist_nbs, twinnodes);

        for(auto node2 : unique_nodes){
            if(unique_check[node2])
                continue;
            unique_all.insert(node2);
            unique_new.push(node2);
            unique_check[node2] = 1;
        }
        if(unique_it.empty()){
            // Store statistic
            unique_level.push_back(unique_all.size()); 
            // Update
            unique_it = unique_new;
            unique_new = {}; 
        }
    }
    unique_count = unique_all.size();
    runtime_tot = ((double)(clock() - (double)time_tot))/CLOCKS_PER_SEC;
    return unique_all;
}

std::set<int> AnonymityCascade::subgraph_cascade(Graph *g, EquivalencePartition *EP1, EquivalencePartition *EP2, bool twinnodes, const int level){
    // Initial anonymity
    std::map< std::pair<int, int>, int> casc_dict;
    auto node_to_ep = EP1->get_node_to_ep_entry();
    int ep_id1, ep_id2;
    std::set<int> unique_set;
    int k_value;

    // Get occurrence pairs ep - ep
    for(auto edge : g->get_edges()){
        ep_id1 = node_to_ep[edge.first];
        ep_id2 = node_to_ep[edge.second];
        casc_dict[std::make_pair(ep_id1, ep_id2)] += 1;
    }

    for(auto edge : g->get_edges()){
        ep_id1 = node_to_ep[edge.first];
        ep_id2 = node_to_ep[edge.second];
        k_value = casc_dict[std::make_pair(ep_id1, ep_id2)];

        if(k_value == 1){
            unique_set.insert(ep_id1);
            unique_set.insert(ep_id2);
        }
    }
    return unique_set;
}

std::set<int> AnonymityCascade::anonymity_cascade(Graph *g, EquivalencePartition *EP, const bool twinnodes, const int level){
    return anonymity_cascade(g, EP, EP, twinnodes, level);
}

const void AnonymityCascade::print_info(){
    std::cout<<"Starting measure: "<<std::endl;
    std::cout<<"Cascade measure: "<<std::endl;
    std::cout<<"Maximum distance: "<<std::endl;
}

const void AnonymityCascade::print_stats(const int stat){
    
    if(stat >= 1){
        std::cout<<"Unique nodes per level:"<<std::endl;
        for(int i= 0; i < unique_level.size(); i++){
            std::cout<<"Unique nodes level "<<i<<": "<<unique_level[i]<<std::endl;
        }
    }
    std::cout<<"Unique nodes total: "<<unique_count<<std::endl;
    if(stat >=2){
        std::cout<<"Runtime total: "<<runtime_tot<<std::endl;
    }
}


