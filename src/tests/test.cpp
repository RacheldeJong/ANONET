
/*
    Test functionalities of Graph class
*/
#include "test.h"

void graphutil_test(sparsegraph sg){
    std::cout<<"Start of graphutil test"<<std::endl;
    Graph g = Graph(sg);
    int src_node = 0;
    int d = 1;

    // Print nodes + edges input graph
    std::cout<<"Number nodes and edges"<<std::endl;
    std::cout<<"Nodes: "<<g.get_number_nodes()<<std::endl; 
    std::cout<<"Edges: "<<g.get_number_edges()<<std::endl;
    std::cout<<"Directed: "<<g.is_directed()<<std::endl;
    std::cout<<"The graph:"<<std::endl;
    g.print_graph();

    // Print degree dist.
    std::cout<<"Degree distribution"<<std::endl;
    auto deg_dist = g.get_degree_distribution();
    for(auto deg_it : deg_dist) 
        std::cout<<"["<<deg_it.first<<", "<<deg_it.second<<"], ";
    std::cout<<std::endl;

    // Print all nodes
    auto nodes = g.get_nodes();
    for(auto node : nodes) std::cout<<node<<", ";
    std::cout<<std::endl;

    // Print all edges
    auto edges = g.get_edges();
    for(auto edge : edges) std::cout<<"("<<edge.first<<", "<<edge.second<<"), ";
    std::cout<<std::endl;

    // Print hash
    std::cout<<"Hash"<<std::endl;
    long hash;
    hash = g.get_hash();
    std::cout<<hash<<std::endl;

    // Print neighborhood nodes and neighborhood
    std::cout<<"Get neighborhood of node 0"<<std::endl;
    int nr_edges;
    std::unordered_set<int> node_set;
    node_set = g.get_neighborhood_nodes(src_node, d, nr_edges);
    for(auto node : node_set) std::cout<<node<<" ";
    std::cout<<std::endl;
    Graph nbh = g.get_neighborhood(src_node, d);
    std::cout<<nbh.get_number_nodes()<<" "<<nbh.get_number_edges()<<std::endl;
    nbh.print_graph();

    std::set <int> node_vec = g.get_nodes();
    // Compute all neighborhoods
    for(size_t d=1; d<=5; d++){
        std::cout<<"d="<<d<<std::endl;
        for(auto ego : node_vec)
            g.get_neighborhood(ego, d);
    }

    // Print giant component nodes
    std::cout<<"Giant component nodes"<<std::endl;
    std::set <int> node_vec2 = g.get_giant_component_nodes();
    for(auto node_it : node_vec2) std::cout<<node_it<<" ";
    std::cout<<std::endl;
    g.filter_giant_component();

    std::cout<<"New node set: "<<std::endl;
    node_vec2 = g.get_nodes();
    std::cout<<"Size difference: "<<node_vec.size() - node_vec2.size()<<std::endl;
    for(auto it : node_vec2) std::cout<<it<<", ";
    std::cout<<std::endl;

    std::cout<<"End of graphutil test"<<std::endl;
}


void cache_test(sparsegraph sg){
    std::cout<<"Start of cache test"<<std::endl;

    Graph g = Graph(sg);
    Cache cache = Cache(0);
    int v = 1;
    cache_pair val;

    // Compute cache values
    std::cout<<"Computing cache values first time"<<std::endl;
    for(size_t i = 0; i < g.get_number_nodes(); i++){
        val = cache.get_cache_entry(i);
        cache.print_val_node(i);
    }
    std::cout<<"Cache size "<<cache.get_cache_size()<<std::endl;

    // Update entry
    // TODO: alter graph
    std::cout<<"Updating"<<std::endl;
    cache.update_cache_entry(v);

    // Get values from cache
    std::cout<<"Getting values from cache"<<std::endl;
    for(size_t i = 0; i < g.get_number_nodes(); i++){
        cache.print_val_node(i);
    }
    std::cout<<"Cache size "<<cache.get_cache_size()<<std::endl;

    std::cout<<"Checking for equivalence"<<std::endl;
    for(size_t i = 0; i < g.get_number_nodes(); i++){
        for(size_t j = 0; j < g.get_number_nodes(); j++){
            std::cout<<"Nodes "<<i <<", "<<j<<" : ";
            std::cout<<cache.are_equivalent(i, j)<<std::endl;
        }
    }
    std::cout<<"End of cache test"<<std::endl;
}

void equivalence_test(sparsegraph sg){
    std::cout<<"Start of equivalence test"<<std::endl;
    std::vector<int> measures = {MEAS_DEGREE, MEAS_COUNT, MEAS_DKANON};
    int max_distance = 2;

    Graph g = Graph(sg);
    // EquivalencePartition eq = EquivalencePartition(&g, std::make_pair(1, 5), 1);
    for(int i = 0; i < 3; i++){
        int meas = measures[i];
        EquivalencePartition eq = EquivalencePartition(&g, std::make_pair(meas, 2));
        eq.print_info();
        eq.get_equivalence_partition();
        eq.print_equivalence_partition();
        eq.print_equivalence_distribution();
        eq.print_equivalence_distribution(false);
        eq.print_node_anonymity();
        eq.print_node_anonymity(false);

        EquivalencePartition *eq_dist = eq.get_distance_partition(1);
        std::cout<<"EP d=1 with and without twin nodes"<<std::endl;
        eq_dist->print_info();
        eq_dist->print_equivalence_partition();
        eq_dist->print_equivalence_distribution();
        eq_dist->print_equivalence_distribution(false);
        eq_dist->print_node_anonymity();
        eq_dist->print_node_anonymity(false);
    }

    EquivalencePartition eq = EquivalencePartition(&g, MEAS_DKANON);
    auto ep_cur = eq.get_equivalence_partition();
    auto rep_nodes = eq.get_representative_nodes();
    std::set<int> eq_found;
    std::cout<<"Representative nodes: ";
    for(auto it : rep_nodes){
        std::cout<<it<<", ";
        eq_found.insert(eq.get_eq_entry(it));
        if(ep_cur.size() < eq.get_eq_entry(it)){
            std::cout<<"eq entry out of bounds"<< eq.get_eq_entry(it)<<std::endl;
        }
    }
    std::cout<<std::endl;
    std::cout<<"Eq test, the value should be 1: ";
    std::cout<< (eq_found.size() == ep_cur.size()) <<std::endl;

    std::cout<<"End of equivalence test"<<std::endl;
}

void twin_test(sparsegraph sg){
    Graph g = Graph(sg);
    std::cout<<"Start of equivalence test"<<std::endl;
    
    // Through twin nodes object
    Twinnodes t = Twinnodes(&g);
    t.compute();
    auto twins = t.get_twinnodes();
    auto twins_minimal = t.get_minimal_twins();
    auto node_to_twin = t.get_node_to_twin();

    std::cout<<"Twin nodes: "<<std::endl;
    for(auto twin_it : twins){
        std::cout<<"\t";
        for(auto node : twin_it)
            std::cout<<node<<", ";
        std::cout<<std::endl;
    }

    std::cout<<"Minimal twins: "<<std::endl;
    for(auto node : twins_minimal){
        std::cout<<node<<", ";
    }
    std::cout<<std::endl;

    std::cout<<"Node to twins: "<<std::endl;
    for(auto node : node_to_twin){
        std::cout<<node<<", ";
    }
    std::cout<<std::endl;

    std::cout<<"Nr twins: "<<t.get_nr_twinnodes()<<std::endl;
    std::cout<<"Nr nontwins: "<<t.get_nr_nontwinnodes()<<std::endl;
    std::cout<<"Test if equal nodes: ";
    std::cout<< (t.get_nr_twinnodes() + t.get_nr_nontwinnodes() == g.get_number_nodes())<<std::endl;

    // Through graph object
    g.init_twin();
    
    twins = g.twinnodes->get_twinnodes();
    twins_minimal = g.twinnodes->get_minimal_twins();
    node_to_twin = g.twinnodes->get_node_to_twin();

    std::cout<<"Twin nodes: "<<std::endl;
    for(auto twin_it : twins){
        std::cout<<"\t";
        for(auto node : twin_it)
            std::cout<<node<<", ";
        std::cout<<std::endl;
    }

    std::cout<<"Minimal twins: "<<std::endl;
    for(auto node : twins_minimal){
        std::cout<<node<<", ";
    }
    std::cout<<std::endl;

    std::cout<<"Node to twins: "<<std::endl;
    for(auto node : node_to_twin){
        std::cout<<node<<", ";
    }
    std::cout<<std::endl;

    std::cout<<"End of twin node test"<<std::endl;
}

// Test add and delete function by for each edge deleting and adding
// it to the graph
// TODO: If add is extended, add more tests
void del_add_test(sparsegraph sg){
    std::cout<<"Start delete / add test"<<std::endl;
    Graph g = Graph(sg);
    std::set<int> neighbors;
    std::cout<<"Before: "<<std::endl;
    g.print_graph();

    for(auto src : g.get_nodes()){
        neighbors = g.get_neighbors(src);

        for(auto trgt : neighbors){
            std::cout<<"Deleting and adding edge "<< src<<" - "<<trgt<<std::endl;
            g.delete_edge(src, trgt);
            g.add_edge(src, trgt);
            g.print_graph();
        }
    }
    std::cout<<"After: "<<std::endl;
    g.print_graph();

    // TODO: test for adding edges that do not fit

    std::cout<<"End of delete / add test"<<std::endl;
}

void diff_test(sparsegraph sg){
    Graph g = Graph(sg);
    Graph g_sub, g_sub_del;
    SG_DECL(sub);
    SG_ALLOC(sub, sg.nv, sg.nde, "malloc");
    int nbh_radius = 1;
    int dk_value;
    

    // Graph editing distance graph
    std::vector<int> node_set, cache_vals;
    std::set<int> ged_nodes; // Set of nodes (hash values found)
    std::map<int, std::vector<int> > ged_graph; // map original hash value to neighbor values
    dkAnonymity dk_computer = dkAnonymity(&g);
    clock_t t1, t2;
    
    // Compute cache value for each node
    EquivalencePartition ep = EquivalencePartition(&g, 2);
    ep.get_equivalence_partition();

    node_set = ep.get_representative_nodes();

    std::cout<<"Start"<<std::endl;
    t1 = clock();
    
    int sim_node_count_1 = 0;
    int sim_node_count_2 = 0;
    // Iterate over each node
    for(auto node : node_set){
        int node_copy = node;
        int sim_count_1 = 0;
        int sim_count_2 = 0;

        // Get neighborhood
        g_sub = g.get_neighborhood(node_copy, nbh_radius);
        ged_nodes.insert(ep.cache.get_cache_entry(node).first);

        // Delete each edge
        std::vector< std::pair<int, int > > edges = g_sub.get_edges();
        //std::cout<<g_sub.get_number_nodes()<< " "<< g_sub.get_number_edges()<<std::endl;

        for(auto edge : edges){
            // Delete edge
            g_sub.delete_edge(edge.first, edge.second);
            // Compute new value
            dk_value = dk_computer.compute_value(&g_sub, node_copy).first;
            
            //std::cout<<"dk_value: "<<dk_value<<std::endl;

            // Add to GED graph
            if(ged_nodes.find(dk_value) != ged_nodes.end()){
                sim_count_1 += 1;
            }
            //ged_nodes.insert(dk_value);

            // Do some checks

            // Add edge back
            g_sub.add_edge(edge.first, edge.second);
        }
        if(sim_count_1 > 0){
            sim_node_count_1 += 1;
        }
        std::cout<<"simcount "<<node<<" = "<< sim_count_1 <<" "<<sim_count_2<< " "<<sim_node_count_1<<" "<<sim_node_count_2<<std::endl;


        continue;

        for(auto edge1 : edges){
            // Delete edge
            g_sub.delete_edge(edge1.first, edge1.second);
            
            for(auto edge2 : edges){
                if((edge1.first == edge2.first && edge1.second == edge2.second)
                || (edge1.first == edge2.second && edge1.second == edge2.first)){
                    continue;
                }

                // Delete edge
                g_sub.delete_edge(edge2.first, edge2.second);
            
                // Compute new value
                dk_value = dk_computer.compute_value(&g_sub, node_copy).first;


                
                //std::cout<<"dk_value: "<<dk_value<<std::endl;

                // Add to GED graph
                if(ged_nodes.find(dk_value) != ged_nodes.end()){
                    sim_count_2 += 1;
                }
                //ged_nodes.insert(dk_value);

                // Do some checks
                 // Add edge back
                g_sub.add_edge(edge2.first, edge2.second);
            }
            // Add edge back
            g_sub.add_edge(edge1.first, edge1.second);
        }

        if(sim_count_1 > 0){
            sim_node_count_1 += 1;
        }
        if(sim_count_2 > 0){
            sim_node_count_2 += 1;
        }
        std::cout<<"simcount "<<node<<" = "<< sim_count_1 <<" "<<sim_count_2<< " "<<sim_node_count_1<<" "<<sim_node_count_2<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"Sim node count 1: "<<sim_node_count_1<<std::endl;
    std::cout<<"Sim node count 2: "<<sim_node_count_2<<std::endl;
    std::cout<<"Time: "<< ((double)(clock() - (double)t1))/CLOCKS_PER_SEC <<std::endl;
}

void heuristic_test(sparsegraph sg){
    std::cout<<"Start of heuristic test"<<std::endl;
    
    Graph g = Graph(sg);
    int d = 2;
    // EquivalencePartition eq = EquivalencePartition(&g, std::make_pair(1, 5), 1);
    //EquivalencePartition eq = EquivalencePartition(&g, MEAS_DKANON, HEUR_DKANON);
    EquivalencePartition eq = EquivalencePartition(&g, std::make_pair(MEAS_DKANON, d), HEUR_DKANON);

    eq.print_info();

    eq.get_equivalence_partition();
    std::cout<<"This should give an error:"<<std::endl;
    EquivalencePartition *heur_ep = eq.get_heuristic_partition(3);

    std::cout<<"End of heuristic test"<<std::endl;
}



std::pair<int, int> select_edge_random(Graph *g, EquivalencePartition *eq, std::vector<std::pair<int, int>> edges){
    int edge_nr = std::rand() % edges.size();
    return edges[edge_nr];
}

void anonymization(sparsegraph sg){
    int meas_main = MEAS_COUNT;
    int meas_heur = MEAS_DEGREE;
    int d = 2;
    int recompute_gap = 5;
    int budget = 500000;
    int it_nr = 0;

    Graph g = Graph(sg);
    EquivalencePartition eq = EquivalencePartition(&g, std::make_pair(meas_main, d), meas_heur);
    auto edges = g.get_edges();

    std::cout<<"Initial anonymity distribution: "<<std::endl;
    eq.print_anonymity_distribution();

    while(it_nr < budget && edges.size() > 0){
        auto edge = select_edge_random(&g, &eq, edges);
        g.delete_edge(edge.first, edge.second);
        it_nr += 1;

        if(it_nr % recompute_gap == 0){
            eq.update_ep();
            edges = g.get_edges();
            std::cout<<"New anonymity distribution: "<<std::endl;
            eq.print_anonymity_distribution();
            recompute_gap *= 2;
        }
    }
    eq.print_equivalence_partition();
}

void update_test(sparsegraph sg){
    anonymization(sg);
    return;
    
    int d = 2;
    Graph g = Graph(sg);

    auto edges = g.get_edges();
    EquivalencePartition eq = EquivalencePartition(&g, std::make_pair(MEAS_COUNT, d), MEAS_COUNT);
    int tel = 5;

    for(auto it : edges){
        g.delete_edge(it.first, it.second);
        eq.update_ep();
        std::cout<<"New EP:"<<std::endl;
        eq.print_equivalence_partition();
    }
}