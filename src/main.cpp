// main.cpp:
// Author: Rachel de Jong
// 
// Last edited: 
//

#include "util.h"
#include "graph/nauty_graph.h"
#include "graph/graphgen.h"
#include "equivalence/equivalence.h"
#include "anonalg/anonymization.h"
#include "equivalence/cascade.h"

std::string meas_options = "NONE 0, DEGREE 1 COUNT 2, DEGDIST 3, DKANON 4, HAY 5";

// Default settings
bool use_menu = false;
int cascade = 0;
int anonymize = 0;
int meas_main = MEAS_DKANON;
int meas_main2 = MEAS_DKANON;
int meas_heur = MEAS_DEGREE;
int meas_heur2 = MEAS_DEGREE;
int distance = 1;
int iterative = 1;
int anon_alg = 0;
int k = 2;
int twin_uniqueness = 0;
int budget = -1;
int recompute_gap = -1;
int statistics = 0;
char *input_file;

void menu(){
    std::string option, input_file;
    bool menustart = true;
    int n, m;
    SG_DECL(sg);

    AnonymityCascade c = AnonymityCascade();
    EquivalencePartition *EP = NULL;
    Anonymization *a = NULL;
    Graph g;
    int graph_status = 0; // 0: not initialized, 1: initialized no EP, 2: yes EP
    bool change_settings = false;

    while(true){
        std::cout<<"Options:"<<std::endl;
        std::cout<<"  I to read graph from input file"<<std::endl;
        std::cout<<"  E for computing partition"<<std::endl;
        std::cout<<"  C for anonymity cascade"<<std::endl;
        std::cout<<"  A for anonymization\n"<<std::endl;
        std::cout<<"  P to print graph\n"<<std::endl;
        std::cout<<"To change options use: D, OE, OC, OA"<<std::endl;
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
            c.anonymity_cascade(&g, EP, &EP2, twin_uniqueness, -1);
            c.print_stats(2);
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
            if(budget == -1) budget = ceil(0.05 * g.get_number_edges());
            if(recompute_gap == -1) recompute_gap = ceil(0.01 * budget);
            if(a == NULL) a = new Anonymization(&g, EP, anon_alg, budget, recompute_gap);
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
            std::cout<<"Budget: ";
            std::cin>>budget;
            std::cout<<"Recompute gap ";
            std::cin>>recompute_gap;
            if(a != NULL){
                a->~Anonymization();
                delete a;
            } 
            a = new Anonymization(&g, EP, anon_alg, budget, recompute_gap);
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
        // Input file
        else if(strcmp(argv[i], "-i") == 0){
            input_file = (argv[i + 1]);
            i++;
        }
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
            budget = atoi(argv[i+1]);
            i++;
        }
        // Update after every recompute_gap edges
        else if(strcasecmp(argv[i], "-u") == 0){
            recompute_gap = atoi(argv[i+1]);
            i++;
        }
    }
}

// Main
int main(int argc, char *argv[]){
    // Initialize variables
    SG_DECL(sg1);
    int m, n = 0;
    char *input_file;

    // Parse input and read graph
    if(argc > 1) parse_input(argc, argv);

    if(argv[1][0] != '-'){
        input_file = argv[1];
        n = read_n(input_file);
        m = SETWORDSNEEDED(n);
        sg1 = read_graph_from_file(input_file, n);
    }

    if(use_menu){
        menu();
        return(0);
    }

    Graph g = Graph(sg1);
    std::cout<<"|V|"<<g.get_number_nodes() <<" |E|: "<<g.get_number_edges()<<std::endl;

    EquivalencePartition ep = EquivalencePartition(&g, std::make_pair(meas_main, distance), meas_heur);
    
    if(statistics > 1 || (!cascade && !anonymize))ep.print_info();
    if(statistics > 0)ep.print_anonymity_distribution(twin_uniqueness);
    if(statistics > 0)ep.print_equivalence_distribution(twin_uniqueness);
    if(cascade){
        EquivalencePartition ep2 = EquivalencePartition(&g, std::make_pair(meas_main2, distance), meas_heur2);
        AnonymityCascade c = AnonymityCascade();
        c.anonymity_cascade(&g, &ep, &ep2);
        std::cout<<"Cascading results:"<<std::endl;
        c.print_stats(statistics);
    }
    if(anonymize){
        if(budget == -1) budget = ceil(0.1 * g.get_number_edges());
        if(recompute_gap == -1) recompute_gap = ceil(0.1 * budget);
        Anonymization a = Anonymization(&g, &ep, anon_alg, budget, recompute_gap);
        a.anonymize();
        if(statistics > 1)a.print_info();
        a.print_statistics();
    }

    SG_FREE(sg1);
    
    return 0;
}
