#include "experiments.h"

void do_experiment(const int choice, char *input_file, sparsegraph sg1){
    
    srand(RAND_SEED_EXP);
    switch (choice){
        case EXP_FUZZY:
            std::cout<<"Experiment: fuzzy"<<std::endl;
            exp_fuzzy_anonymization(input_file, sg1);
            std::cout<<"Experiment done."<<std::endl;
            break;
        case EXP_EXAMPLE:
            std::cout<<"Start example experiment"<<std::endl;
            example_experiment(input_file, sg1);
            std::cout<<"Example experiment done."<<std::endl;
            break;
        default:
            std::cout<<"Not an experiment option."<<std::endl;
            break;
    }
}

void exp_fuzzy_anonymization(char *input_file, sparsegraph sg1){
    std::vector<int> distances = {1};
    std::vector<int> algs = {ALG_RAND, ALG_AFF_UNIQUE};
    std::vector<int> measures = {MEAS_COUNT};
    std::vector<int> fuzzyness = {0, 1, 5, 10, 20, 30}; 
    int meas = measures[0];
    int n = sg1.nv;
    sg1 = read_graph_from_file(input_file, n);
    int budget = sg1.nde * 0.50 * 0.05; // Budget: 10% of all edges
    int recompute_gap = ceil(budget / 20);
    int iterations = 5;
    bool perc = true;

    std::cout<<"Nodes: "<<sg1.nv<<std::endl;
    std::cout<<"Edges: "<<sg1.nde / 2<<std::endl;
    std::cout<<"Budget: "<<budget<<std::endl;
    std::cout<<"Recompute gap: "<<recompute_gap<<std::endl;
    // Iterate over options
    for(int i = 0; i < iterations; i++){
        for(auto d : distances){
            for(auto alg : algs){
                for(auto f : fuzzyness){
                    for(bool use_fuzzy : {true}){
                        std::cout<<"Phi: "<<f<<std::endl;
                        sg1 = read_graph_from_file(input_file, n);
                        Graph g = Graph(sg1);
                        EquivalencePartition ep = EquivalencePartition(&g, std::make_pair(meas, d));
                        // Perform anonymization and print statistics
                        Anonymization anon = Anonymization(&g, &ep, alg, budget, recompute_gap);
                        anon.set_fuzzy_anon(f, perc, use_fuzzy);
                        anon.print_info();
                        anon.anonymize();
                        anon.print_statistics();
                        anon.print_altered_edges();
                    }
                }
            }
        }
    }
}

void example_experiment(char *input_file, sparsegraph sg1){
    // Initialize
    int n = sg1.nv;
    Graph g = Graph(sg1);

    // Computations

    // Output
}