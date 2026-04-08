#include "../equivalence/equivalence.h"
#include "../util.h"

sparsegraph *subgraph;

int read_n(const char* file_name){
    FILE* file;
    char c;
    int n = 0;

    if((file = fopen(file_name, "r")) == NULL){ 
        printf("Error: File %s could not be opened\n", file_name);
        return 0;
     }
     // Read n
    while (c != EOF){
        if(c == '\n' && (c=getc(file)) != '!') break;
        if(c == 'n' && (c=getc(file)) == '='){
           c = getc(file);
           while (ISDIGIT(c)){
              n *= 10;
              n += c - '0';
              c = getc(file);
           }
         }
         else c = getc(file);
    }
    return n;
}

void parse_input(int argc, char *argv[]){
   // TODO
}

void print_measure(const int meas_choice){
    if(meas_choice == MEAS_DEGREE) std::cout<<"Degree"<<std::endl;
    else if(meas_choice == MEAS_DEGDIST) std::cout<<"Degree distribution"<<std::endl;
    else if(meas_choice == MEAS_HAY) std::cout<<"Hay"<<std::endl;
    else if(meas_choice == MEAS_COUNT) std::cout<<"Count"<<std::endl;
    else if(meas_choice == MEAS_DKANON) std::cout<<"d-k-Anonymity"<<std::endl;
    else std::cout<<"none"<<std::endl;
}

void print_results_ep(EquivalencePartition *ep){
    std::cout<<"Anonymity+twin distribution"<<std::endl;
    ep->print_anonymity_distribution();
    std::cout<<"Anonymity%twin distribution"<<std::endl;
    ep->print_anonymity_distribution(false);
}

void print_results_ep_it(EquivalencePartition *ep, const int max_dist){
    int dist_cur;
    for(size_t i = 0; i < max_dist-1; i++){
        if(!ep->get_iterative()) break;
        dist_cur = i + 1;
        std::cout<<"Start results distance "<<dist_cur<<std::endl;
        EquivalencePartition *dist = ep->get_distance_partition(dist_cur);
        // Print results
        print_results_ep(dist);
    }

    std::cout<<"Start results distance "<<max_dist<<std::endl;
    print_results_ep(ep);
}

void print_graph_summary(Graph g){
    clock_t time_twin;
    std::cout<<"Graph summary"<<std::endl;
    std::cout<<"Nr. nodes: "<<g.get_number_nodes()<<std::endl;
    std::cout<<"Nr. edges: "<<g.get_number_edges()<<std::endl;
    
    time_twin = clock();
    g.init_twin();
    std::cout<<"Nr. twinnodes: "<<g.twinnodes->get_nr_twinnodes()<<std::endl;
    std::cout<<"Nr. repr nodes: "<<(g.twinnodes->get_minimal_twins().size())<<std::endl;
    std::cout<<"Time computing twins: ";
    std::cout<<((double)(clock() - (double)time_twin))/CLOCKS_PER_SEC<<"\n"<<std::endl;
}

void experiment(Graph g){
    //std::vector<int> measures = {MEAS_NONE, MEAS_COUNT, MEAS_DEGREE, MEAS_HAY, MEAS_DKANON}; //, MEAS_HAY}
    
    std::vector<int> measures = {MEAS_NONE, MEAS_DEGREE, MEAS_DEGDIST, MEAS_COUNT, MEAS_HAY, MEAS_DKANON}; //, MEAS_HAY}
    std::vector<bool> iterative = {true};
    int heur;
    int distance = 2;

    print_graph_summary(g);

    for(auto meas : measures){
        print_measure(meas);
        if(meas == MEAS_DKANON) heur = MEAS_COUNT;
        else heur = HEUR_NONE;
        
        for(auto it : iterative){
            std::cout<<"Measure: ";
            print_measure(meas);
            std::cout<<"Distance: "<<distance<<std::endl;
            std::cout<<"Iterative: "<<it<<std::endl;
            EquivalencePartition ep = EquivalencePartition(&g, 
                std::make_pair(meas, distance), heur);
            ep.print_info();
            
            // Set iterative
            ep.set_iterative(it);

            // Compute
            ep.get_equivalence_partition();

            // Print results
            print_results_ep_it(&ep, distance);
            
            // Print runtimes
            ep.print_runtimes();
            std::cout<<"End results measure\n"<<std::endl;
            //ep.print_equivalence_partition();
        }
    }
}

int main(int argc, char *argv[]){

    SG_DECL(sg1); SG_DECL(sg2);
    char *input_file;
    int n, m;

    // Read graph
    if(argc > 1) parse_input(argc, argv);

    // TMP
    input_file = argv[1];
    n = read_n(input_file);
    m = SETWORDSNEEDED(n);
    sg1 = read_graph_from_file(input_file, n);

    Graph g = Graph(sg1);
    experiment(g);
}