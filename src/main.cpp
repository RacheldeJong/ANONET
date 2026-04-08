// main.cpp:
// Author: Rachel de Jong
// 

#include "util.h"
#include "graph/nauty_graph.h"
#include "graph/graphgen.h"
#include "equivalence/equivalence.h"
#include "anonalg/anonymization.h"
#include "equivalence/cascade.h"
#include "experiments/experiments.h"
#include "equivalence/fuzzy.h"
#include "ged/ged.h"

std::string meas_options = "NONE 0, DEGREE 1 COUNT 2, DEGDIST 3, DKANON 4, HAY 5";
std::string alg_options = "TODO";

char *input_file;
// Default settings

// Mode
bool use_menu = false;
int cascade = 0;
int anonymize = 0;
bool experiment = false;

// Experiments
int experiment_choice = 0;

// Measures
int meas_main = MEAS_COUNT;
int meas_heur = MEAS_DEGREE;

// General settings
int phi_val = 0;
int distance = 1;
int iterative = 1;
int k = 2;
int twin_uniqueness = 0;

// Output settings
int statistics = 0;
bool print_altered = false; // Anonymization only
bool print_ep = false;

// Cascading
int meas_main2 = MEAS_COUNT;
int meas_heur2 = MEAS_DEGREE;

// Anonymization
int anon_alg = 0;
int budget_perc = 5;
int recompute_times = 10;

void menu(){
    std::string option, input_file;
    bool menustart = true;
    int n, m;
    int recompute_gap, budget_edges;
    SG_DECL(sg);

    AnonymityCascade *c = NULL;
    EquivalencePartition *EP = NULL;
    Anonymization *a = NULL;
    Graph g;
    int graph_status = 0; // 0: not initialized, 1: initialized no EP, 2: yes EP
    bool change_settings = false;

    while(true){
        std::cout<<"Options:"<<std::endl;
        std::cout<<"Step 1:"<<std::endl;
        std::cout<<"  I to read graph from input file"<<std::endl;
        std::cout<<"Step 2: (optional)"<<std::endl;
        std::cout<<"To change options use: D, OE, OC, OA"<<std::endl;
        std::cout<<"  P to print graph\n"<<std::endl;
        std::cout<<"Step 3: compute!"<<std::endl;
        std::cout<<"  E for computing partition"<<std::endl;
        std::cout<<"  C for anonymity cascade"<<std::endl;
        std::cout<<"  A for anonymization\n"<<std::endl;
        std::cin>>option;

        // Input file
        if(option == "I"){
            std::cout<<"Please give your input file... ";
            std::cin>>input_file;
            char arr[input_file.length()]; 
	        strcpy(arr, input_file.c_str());
            n = read_n(arr);
            m = SETWORDSNEEDED(n);
            sg = read_graph_from_file(arr, n);
            if(sg.nv == 0){
                std::cout<<"Error: graph could not be read. Please try again."<<std::endl;
                graph_status = 0;
                continue;
            }
            g = Graph(sg);
            std::cout<<"Graph read!\n"<<std::endl;
            graph_status = 1;
        }
        else if(option == "E"){
            if(graph_status == 0){
                std::cout<<"Error: graph is not yet initialized."<<std::endl;
                continue;
            }
            EP = new EquivalencePartition(&g, std::make_pair(meas_main, distance), meas_heur);
            EP->print_anonymity_distribution(twin_uniqueness);
            graph_status = 2;
            change_settings = false;
        }
        else if(option == "C"){
            if(graph_status < 2){
                if(graph_status == 0) std::cout<<"Error: graph is not yet initialized."<<std::endl;
                else if(graph_status == 1) std::cout<<"Error: initial EP is not yet inizialized."<<std::endl;
                continue;
            }
            if(change_settings){
                std::cout<<"Error: EP is not initialized for current settings. Choose E."<<std::endl;
                continue;
            }
            EquivalencePartition EP2 = EquivalencePartition(&g, std::make_pair(meas_main2, distance), meas_heur2);
            //c.anonymity_cascade(&g, &EP, &EP2, twin_uniqueness, -1);
            //c.print_statistics();
        }
        else if(option == "A"){
            if(graph_status < 2){
                if(graph_status == 0) std::cout<<"Error: graph is not yet initialized."<<std::endl;
                else if(graph_status == 1) std::cout<<"Error: initial EP is not yet inizialized."<<std::endl;
                continue;
            }
            if(change_settings){
                std::cout<<"Error: EP is not initialized for current settings. Choose E."<<std::endl;
                continue;
            }
            budget_edges = ceil(budget_perc * g.get_number_edges());
            recompute_gap = ceil(budget_edges / recompute_times);
            if(a == NULL) a = new Anonymization(&g, EP, anon_alg, budget_edges, recompute_gap);
            a->anonymize();
            if(statistics > 1)a->print_info();
            a->print_statistics();
        }
        else if(option == "P"){
            if(graph_status == 0){
                std::cout<<"Error: graph is not yet initialized"<<std::endl;
                continue;
            }
            g.print_graph();
        }
        else if(option == "S"){
            std::cout<<"Statistics mode [0, 1, 2]: ";
            std::cin>>statistics;
            change_settings = true;
        }
        else if(option == "OE"){
            std::cout<<"Anonymity measure: "<<meas_options<<" "<<std::endl;
            std::cin>>meas_main;
            std::cout<<"Anonymity measure: "<<meas_options<<" "<<std::endl;
            std::cin>>meas_heur;
            std::cout<<"Level of uncertainty phi: [0 - 100]"<<std::endl;
            std::cin>>phi_val;

            if((meas_main != MEAS_HAY && meas_main < meas_heur) || 
            (meas_main == MEAS_HAY && (meas_heur != MEAS_DEGREE && meas_heur != MEAS_HAY))){
                std::cout<<"WARNING: your choice of heuristic may influence the obtained";
                std::cout<<" equivaelnce partition"<<std::endl;
            }
            if(meas_main == meas_heur){
                std::cout<<"WARNING: the chosen measure is the same as the heuristic"<<std::endl;
            }
            std::cout<<"Distance: ";
            std::cin>>distance;

            std::cout<<"Anonymity % twinnodes [0, 1]: ";
            std::cin>>twin_uniqueness;
            change_settings = true;
        }
        else if(option == "OC"){
            std::cout<<"Cascade measure: "<<meas_options<<std::endl;
            std::cin>>meas_main2;
            std::cout<<"Cascade heuristic measure: "<<meas_options<<std::endl;
            if((meas_main2 != MEAS_HAY && meas_main2 < meas_heur2) || 
            (meas_main2 == MEAS_HAY && (meas_heur2 != MEAS_DEGREE && meas_heur2 != MEAS_HAY))){
                std::cout<<"WARNING: your choice of heuristic may influence the obtained";
                std::cout<<" equivalence partition"<<std::endl;
            }
            if(meas_main2 == meas_heur2){
                std::cout<<"WARNING: the chosen measure is the same as the heuristic"<<std::endl;
            }
        }
        else if(option == "OA"){
            std::cout<<"Anonymization algorithm "<<std::endl;
            std::cin>>anon_alg;
            std::cout<<"Budget%: ";
            std::cin>>budget_edges;
            std::cout<<"Recompute times";
            std::cin>>recompute_gap;
            if(a != NULL){
                a->~Anonymization();
                delete a;
            }
            budget_edges = ceil(budget_perc * g.get_number_edges());
            recompute_gap = ceil(budget_edges / recompute_times);
       
            a = new Anonymization(&g, EP, anon_alg, budget_edges, recompute_gap);
            // Additional settings
            if(phi_val != -1){
                a->set_fuzzy_anon(phi_val, true, true);
                if(meas_main != MEAS_COUNT){
                    std::cout<<"WARNING: fuzzy-k-anonymity is currently only implemented for the COUNT measure ("<<MEAS_COUNT<<")"<<std::endl;
                    std::cout<<"Change measure to COUNT, or regular anonymity is computed"<<std::endl;
                }
            }
            a->set_twin_uniqueness(twin_uniqueness);
        }
        else if(option == "e"){
            break;
        }
    }
}

void parse_input(int argc, char *argv[]){
    for(int i =1; i < argc; i++){
        // Which steps to perform
        if(strcmp(argv[i], "-M") == 0){
            use_menu = true;
        }
        else if(strcmp(argv[i], "-C") == 0){
            cascade = 1;
        }
        else if(strcmp(argv[i], "-A") == 0){
            anonymize = 1;
        }
        // Variables for all
        // Set distance
        else if(strcmp(argv[i], "-d") == 0){
            distance = atoi(argv[i+1]);
            i++;
        }
        // Set minimum value k
        else if(strcasecmp(argv[i], "-k") == 0){
            k = atoi(argv[i+1]);
            i++;
        } 
        // Set uncertainty value (fuzzy anonymity)
        else if(strcasecmp(argv[i], "-f") == 0){
            phi_val = atoi(argv[i+1]);
            i++;
        } 
        // Set twin-uniqueness
        else if(strcasecmp(argv[i], "-t") == 0){
            twin_uniqueness = atoi(argv[i+1]);
            i++;
        }
        // Statistics mode
        else if(strcasecmp(argv[i], "-s") == 0){
            statistics = atoi(argv[i+1]);
            i++;
        }
        else if(strcasecmp(argv[i], "-edges") == 0){
            print_altered = true;
        }
        else if(strcasecmp(argv[i], "-ep") == 0){
            print_ep = true;
        }
        // Equivalence partitions
        // Main measure
        else if(strcasecmp(argv[i], "-me") == 0){
            meas_main = atoi(argv[i+1]);
            i++;
        }
        // Heuristic measure
        else if(strcasecmp(argv[i], "-mh") == 0){
            meas_heur = atoi(argv[i+1]);
            i++;
        }
        // Main measure second EP (cascade)
        else if(strcasecmp(argv[i], "-me2") == 0){
            meas_main2 = atoi(argv[i+1]);
            i++;
        }
        // Heuristic measure second EP (cascade)
        else if(strcasecmp(argv[i], "-mh2") == 0){
            meas_heur2 = atoi(argv[i+1]);
            i++;
        }
        // Iterative computation
        else if(strcasecmp(argv[i], "-it") == 0){
            iterative = atoi(argv[i+1]);
            i++;
        }
        // Anonymization algorithm
        else if(strcasecmp(argv[i], "-a") == 0){
            anon_alg = atoi(argv[i+1]);
            i++;
        }
        // How many edges to delete
        else if(strcasecmp(argv[i], "-b") == 0){
            budget_perc = atoi(argv[i+1]);
            i++;
        }
        // Update after every recompute_gap edges
        else if(strcasecmp(argv[i], "-u") == 0){
            recompute_times = atoi(argv[i+1]);
            i++;
        }
        else if(strcasecmp(argv[i], "-exp") == 0){
            experiment = true;
            experiment_choice = atoi(argv[i+1]);
        }
    }
}

// Main
int main(int argc, char *argv[]){
    // Step 1: Initialize variables
    SG_DECL(sg1);
    int m, n = 0;
    int recompute_gap, budget_edges;
    char *input_file;

    // Step 2: parse input and read graph
    if(argc > 1) parse_input(argc, argv);

    if(argv[1][0] != '-'){
        input_file = argv[1];
        n = read_n(input_file);
        m = SETWORDSNEEDED(n);
        sg1 = read_graph_from_file(input_file, n);
    }

    // Menu mode
    if(use_menu){
        menu();
        return(0);
    }

    // Step 3: read graph
    Graph g = Graph(sg1);
    if(statistics > 0){
        std::cout<<"Graph info"<<std::endl;
        std::cout<<"|V|"<<g.get_number_nodes() <<" |E|: "<<g.get_number_edges()<<std::endl;
    }
    
    // Step 4: compute equivalence
    EquivalencePartition ep = EquivalencePartition(&g, std::make_pair(meas_main, distance), meas_heur);
    
    // Step 5: additional steps
    // Optional: print ep information
    // Always shown if this is the only step
    if(phi_val == 0){
        if(statistics > 0 || (!cascade && !anonymize)){
            ep.print_info();
            ep.print_anonymity_distribution(twin_uniqueness);
        }
        if(statistics>1){
            //ep.print_statistics();
        }
        if(print_ep) ep.print_equivalence_partition();
    }
    else{ // Fuzzy k-anonymity
        Ged ged = Ged(&g, &ep);
        if(meas_main != MEAS_COUNT){
            std::cout<<"Error: fuzzy is currently only implemented for COUNT ["<<MEAS_COUNT<<"]"<<std::endl;
            std::cout<<"Exiting program."<<std::endl;
            exit(1);
        }
        FuzzyEP fuzzy = FuzzyEP(&g, &ep, &ged, phi_val);
        if(statistics > 0 || (!cascade && ! anonymize)){
            fuzzy.print_info();
            fuzzy.print_fuzzy_distribution();
        }
        if(statistics>1){
            fuzzy.print_statistics();
        }
        if(print_ep) fuzzy.print_node_to_equivalent();
    }
    
    // Option 2: Perform cascading and print results
    if(cascade){
        EquivalencePartition ep2 = EquivalencePartition(&g, std::make_pair(meas_main2, distance), meas_heur2);
        AnonymityCascade c = AnonymityCascade(&g, &ep, &ep2);
        c.print_info();
        std::cout<<"Cascading results:"<<std::endl;
        c.print_stats(statistics);
    }

    // Option 3: anonymization
    if(anonymize){
        budget_edges = ceil((float)budget_perc/100 * g.get_number_edges());
        recompute_gap = ceil(budget_edges / recompute_times);
        
        Anonymization anon = Anonymization(&g, &ep, anon_alg, budget_edges, recompute_gap);
        if(phi_val != 0){
            anon.set_fuzzy_anon(phi_val, true, true);
        }
        anon.anonymize();
        if(statistics > 1)anon.print_info();
        anon.print_statistics();
        if(print_altered) anon.print_altered_edges();
    }

    // Option 4: experiment mode
    if(experiment){
        do_experiment(experiment_choice, input_file, sg1);
    }

    // End program
    SG_FREE(sg1);
    return 0;
}
