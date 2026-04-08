#include "util.h"
#include "graph/nauty_graph.h"
#include "graph/graphgen.h"
#include "measures/measure.h"
#include "anonalg/anonymization.h"
#include "equivalence/cascade.h"
// Map edge to set of nodes that will be affected by deleting the edge
#define aff_dict std::map< std::pair<int, int>, std::set<int>> 

#define cache std::map<int, std::pair<int, int>>

/* Need to keep track of:
    cache
    val_to_eq
    ep
    affected_dict
*/ 

// Returns (n, m) for node
std::pair<int, int> compute_node_state(Graph *g, int node){
    auto nbh = g->get_neighborhood(node, node);
    int n = nbh.get_number_nodes();
    int m = nbh.get_number_edges();
    return std::make_pair(n, m);
}

// Generate the cache from scratch
cache init_cache(Graph *g){
    cache c;
    for(auto node : g->get_nodes()){
        auto state = compute_node_state(g, node);
        c[node] = state;
    }
    return c;
}

// Update cache for given value
// TODO: based on edge deleted
void update_cache(Graph *g, cache c, Measure *m, int node){
    auto val = compute_node_state(g, node);
    c[node] = val;    
}

// Compute equivalence partition based on cache
std::pair<
std::vector<std::vector<int>>,
std::map< std::pair<int, int>, int>
> init_ep(cache c){
    std::vector<std::vector<int>> ep;
    std::map< std::pair<int, int>, int> val_to_eq = {};
    for(auto cache_entry : c){
        auto node = cache_entry.first;
        auto val = cache_entry.second;

        // Check if value is in cache
        auto it = val_to_eq.find(val);  
        if(it != val_to_eq.end()){
            auto ec_class = it->second;
            ep[ec_class].push_back(node);
        }
        // No equivalence class found. Create new
        else{
            val_to_eq[val] = ep.size();
            ep.push_back({node});
        }
    }
    return std::make_pair(ep, val_to_eq);
}

void update_ep(int node, cache &c, std::map< std::pair<int, int> , int> &val_to_eq, std::vector<std::vector<int>> &ep){
    auto val_new = c[node];
    auto it = val_to_eq.find(val_new);  

    if(it != val_to_eq.end()){
        auto ec_class = it->second;
        ep[ec_class].push_back(node);
    }
    // No equivalence class found. Create new
    else{
        val_to_eq[val_new] = ep.size();
        ep.push_back({node});
    }
}

void update_node(Graph *g, Measure *m, int node, cache &c, std::map< std::pair<int, int> , int> &val_to_eq, std::vector<std::vector<int>> &ep){
    update_cache(g, c, m, node);
    update_ep(node, c, val_to_eq, ep);
}

// Create map containing for each edge which nodes are affected by deleting it
aff_dict init_edge_aff_map(Graph *g){
    Measure m = Measure();
    Count *meas = new Count(g);
    m.setMeasure(meas);
    aff_dict affected_dict;
    for(auto edge : g->get_edges()){
        auto affected_nodes = m.get_nodes_affected(g, edge.first, edge.second);
        affected_dict[edge] = affected_nodes;
    }
    return affected_dict;
}

// Update map
void update_edge_aff_map(Graph *g, aff_dict &affected_dict, const std::pair<int, int> &edge, Measure *m){
    // Get set of neighbors: edges between ego and neighbors may be affected
    auto neighbors = g->get_neighbors(edge.first);
    auto neighbors2 = g->get_neighbors(edge.second); 
    neighbors.insert(neighbors2.begin(), neighbors2.end());
    // Update state for possibly affected edges
    for(auto nbr : neighbors){
        std::pair<int, int> edge = std::make_pair(edge.first, nbr);
        auto affected_nodes = m->get_nodes_affected(g, edge.first, edge.second);
        affected_dict[edge] = affected_nodes;
    }
}

std::pair<int, int> random(Graph *g){
    return g->get_edges()[0];
}

void anonymize(Graph *g, int budget, Measure *m){
    // Initialize data structures
    auto edge_aff_map = init_edge_aff_map(g);
    auto cache_cur = init_cache(g);
    auto ep = init_ep(cache_cur);

    int deleted = 0;

    // Anonymization process
    while(deleted < budget){
        // Select edges
        auto edge = random(g);

        // Delete edge
        g->delete_edge(edge.first, edge.second);

        // Update dictionaries
        update_edge_aff_map(g, edge_aff_map, edge, m);
        int node = 0;
        update_cache(g, cache_cur, m, node);
    }
}

int main(int argc, char *argv[]){
    SG_DECL(sg1);
    int m_, n_ = 0;
    char *input_file;
    /* Read input and initialize graph*/
    if(argv[1][0] != '-'){
        input_file = argv[1];
        n_ = read_n(input_file);
        m_ = SETWORDSNEEDED(n_);
        sg1 = read_graph_from_file(input_file, n_);
    }
    Graph g = Graph(sg1);
    Measure m = Measure();
    Count *meas = new Count(&g);
  
    // Some statistics
    std::cout<<"|V|"<<g.get_number_nodes() <<" |E|: "<<g.get_number_edges()<<std::endl;
    std::cout<<"Hello world!"<<std::endl;
    return 0;
}