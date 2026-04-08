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
    else if(meas_choice == MEAS_HYBRID) std::cout<<"Hybrid"<<std::endl;
    else std::cout<<"none"<<std::endl;
}

void print_results_ep(EquivalencePartition *ep){
    //std::cout<<"Partition including twins"<<std::endl;
    //ep->print_equivalence_partition();
    std::cout<<"Distribution including twins"<<std::endl;
    ep->print_anonymity_distribution();
    std::cout<<"Distribution excluding twins"<<std::endl;
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
    
    std::vector<int> measures = {MEAS_NONE, MEAS_DEGREE, MEAS_DEGDIST, MEAS_COUNT, MEAS_HAY, MEAS_DKANON, MEAS_HYBRID}; //, MEAS_HAY}
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

const std::unordered_set<int> get_neighborhood_nodes(sparsegraph g, const int v, const int d, int &edges){
   std::queue<int> q; // Queue with nodes to be visited
   std::unordered_set<int> vis; //Visited nodes (unordered_set for quick access when in set)
   
   int cur_node;
   int depth = 0;
   int time_to_increase_depth = 1;
   edges = 0;
   vis.clear();
   q.push(v);
   vis.insert(v);
   

   // Iterate over nodes to be visited
   while(!q.empty()){
        
      cur_node = q.front();
      // Keep track of when to increase depth of search
      if((time_to_increase_depth -= 1) == 0){
         if(++depth > d) break;
         time_to_increase_depth = q.size();
      }
      q.pop();

      // Iterate over edges of current node
      int pos = g.v[cur_node];
      for(int i =0; i<g.d[cur_node]; i++){
         int nbr_node = g.e[pos+i];
         edges++;
         // Do not add to queue again if node is already added
         if(vis.find(nbr_node) != vis.end()) continue;
         q.push(nbr_node);
         vis.insert(nbr_node);
      }
   } 
   return vis;
}

void get_neighborhood_nauty(sparsegraph g, sparsegraph *new_graph, int &v, const int d){   
   std::unordered_map<int, int> map; // Maps each node in sg to new node in sg2
   int deg, pos, pos1, to, edges;

   // Get nodes in k-neighborhood
   std::unordered_set<int> nodes;
   nodes = get_neighborhood_nodes(g, v, d, edges);
   
   int n = nodes.size();
   int m = SETWORDSNEEDED(n);

   // Initialize sub-graph
   new_graph->nv = n; // Set number of nodes
   new_graph->nde = 0;
   
   // Initialize map sg->sg2
   n = 0;
   for(auto it : nodes){
      map[it] = n;
      n++;
   }

   pos = 0;

   // For each node in the subgraph
   for(auto it : nodes){
      deg = 0;
      new_graph->v[map[it]] = pos; // Set position of node in sg2
      pos1 = g.v[it]; 
      
      // Iterate over edges from it in original graph
      for(int i = 0; i < g.d[it]; i++){
         to = g.e[pos1 + i];

         // Check if edge is contained in subgraph
         if(nodes.find(to) == nodes.end()) continue;

         // Otherwise add edge to subgraph
         new_graph->nde++;
         new_graph->e[pos + deg] = map[to];
         deg++;
      }

      new_graph->d[map[it]] = deg;
      pos += deg;
   }
   v = map[v]; // Set v to corresponding node in new graph
}

std::vector< std::set<int> > split_eclass(sparsegraph g, std::set<int> eclass, int dist){
    std::vector< std::set<int> > ec_split;
    std::map<int, int> node_to_eq;
    std::vector<int> candidate_classes;
    bool ec_found;
    int v_copy, ec_class;
    std::pair<int, int> val;
    std::map<int, int> value;

    std::map<std::pair<int, int>, int> val_to_eq = {};

    int tel = 0;
    int progress = 10;

    if(eclass.size() == 1) return {eclass};

    // Find eq for each node in current eq
    for(auto node1 : eclass){
        v_copy = node1;

        if(tel > progress){
            std::cout<<"Node count: "<<tel<<std::endl;
            progress *= 10;
        }
        tel+=1;

        v_copy = node1;
        get_neighborhood_nauty(g, subgraph, v_copy, dist);
        int nodes = subgraph->nv;
        int edges = subgraph->nde;
        
        
        /*value[N_POS_CACHE] = nodes;
        value[M_POS_CACHE] = edges;
        val = std::make_pair(nodes, edges);
        

        // Comp: log(n) in nr. equivalence classes
        auto it = val_to_eq.find(val);
        if(it != val_to_eq.end()){
            ec_class = it->second;
            ec_split[ec_class].insert(node1);
        }
        // No equivalence class found. Create new
        else{
            val_to_eq[val] = ec_split.size();
            ec_split.push_back({node1});
        }
        */
    }
    return ec_split;
}

void test(sparsegraph g){
    std::vector< std::set< int > >ep_cur, ep_new;
    std::vector<int> minimal_nodes;
    std::vector<int> node_set;
    size_t i;
    int it_nr = 1;
    int max_dist = 5;

    clock_t time_it, time_tot;
    // Start timer total time
    time_tot = clock();

    // Initialize eq
    std::set<int> nodes;
    for(int i =0; i<g.nv; i++){
        nodes.insert(i);
    }
    ep_cur = {nodes};

    for(int cur_dist = 1; cur_dist <= max_dist; cur_dist++){
        // Start timer current iteration
        time_it = clock();

        // Compute equivalence classes by splitting each eq
        for(auto eq : ep_cur){

            for(auto it2 : split_eclass(g, eq, cur_dist)){
                ep_new.push_back(it2);
            }
        }
    }

    fflush(stdout);

}

int main(int argc, char *argv[]){

    SG_DECL(sg1); SG_DECL(sg2);
    char *input_file;

    int n = 69413;
    int m = 3289695;
    SG_DECL(test_graph);
    SG_ALLOC(test_graph, n, m, "malloc");
    subgraph = &test_graph;

    // Read graph
    if(argc > 1) parse_input(argc, argv);

    // TMP
    input_file = argv[1];
    n = read_n(input_file);
    m = SETWORDSNEEDED(n);
    sg1 = read_graph_from_file(input_file, n);

    Graph g = Graph(sg1);
    experiment(g);
    //test(sg1);
}