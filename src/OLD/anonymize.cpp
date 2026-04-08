// main.cpp:
// Author: Rachel de Jong
// 
// Last edited: 
//

#include "graph/nauty_graph.h"
#include "./equivalence/equivalence.h"
#include "./anonalg/anonymization.h"
#include "util.h"

// Read number of nodes from file
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

// Main
int main(int argc, char *argv[]){
    // Settings
    int meas_main = MEAS_COUNT;
    int meas_heur = MEAS_DEGREE;
    int anon_alg = ALG_RAND;
    int budget = 50;
    int recompute_gap = 5;
    int d = 1;

    // Initialize variables
    SG_DECL(sg1); SG_DECL(sg2);
    int m, n = 0;
    char *input_file;

    // Read graph
    if(argc > 1) parse_input(argc, argv);
    input_file = argv[1];
    n = read_n(input_file);
    m = SETWORDSNEEDED(n);
    sg1 = read_graph_from_file(input_file, n);
    Graph g = Graph(sg1);

    // Compute EP
    EquivalencePartition EP = EquivalencePartition(&g, std::make_pair(meas_main, d), meas_heur);

    // Perform anonymization and print statistics
    Anonymization anon = Anonymization(&g, &EP, anon_alg, budget, recompute_gap);
    anon.print_statistics();

    SG_FREE(sg1);
    SG_FREE(sg2);
    
    return 0;
}
