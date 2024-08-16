/* 
    nauty_graph.h
    Class for Graph object, wrapper for nauty sparsegraph
*/ 

#ifndef NAUTYGRAPH_H
#define NAUTYGRAPH_H

#include "../util.h"
#include "graphgen.h"
#include "twinnode.h"

#define HASHKEY 42
#define GIANT_FRAC_THRESHOLD 0.2
#define EMPTY_GRAPH_NODES 0
#define EMPTY_GRAPH_EDGES 0

class Twinnodes;

int read_n(const char* file_name);

class Graph{
    private:

        /* 
            Nauty sparsegraph objects. Use graph_r, containing reversed
            edges, if directed
        */
        sparsegraph graph;
        sparsegraph graph_r;

        /* 
            Mode
        */
        bool directed;

        /* 
            List of nodes in graph
        */
        std::set<int> nodes;

        /*
            When graph is altered, each changed edge is stored here
        */
        std::vector< std::pair<std::pair<int, int>, int> > updated_edges;


        /*
            True if twin nodes have been computed for graph, otherwise false
        */
        bool twin_init;
        
    public:

        Graph *subgraph;

        /* 
           Constructors: construct from given sparsegraphs
           - Empty graph
           - Undirected graph
           - Directed graph
        */
        Graph(){
            graph = get_empty_graph(1);
            directed = true;
            for(int i = 0; i < graph.nv; i++) nodes.insert(i);
            twin_init = false;

            

        };

        Graph(int bloep){
            graph = get_empty_graph(1);
            directed = true;
            for(int i = 0; i < graph.nv; i++) nodes.insert(i);
            twin_init = false;

            

        };
        Graph(const std::string input_file){
            const char * input_file_ = input_file.c_str();
            int n = read_n(input_file_);
            sparsegraph g = read_graph_from_file(input_file_, n);
            graph = g;
            directed = false;
            for(int i = 0; i < g.nv; i++) nodes.insert(i);
            twin_init = false;
        };

        Graph(sparsegraph g){
            graph = g;
            directed = false;
            for(int i = 0; i < g.nv; i++) nodes.insert(i);
            twin_init = false;
        };
        Graph(sparsegraph &g, sparsegraph &g_r){
            graph = g;
            graph_r = g_r;
            directed = true;
            for(int i = 0; i < g.nv; i++) nodes.insert(i);
            twin_init = false;
        };

        /* 
            Destructor
        */
        ~Graph(){};

        Twinnodes *twinnodes;
        void init_twin();

        /* 
            Statistics and settings
            Note: for undirected graphs, each edge is counted twice
        */
        const int get_number_nodes();
        const int get_number_edges();
        const bool is_directed();

        /*
            Returns if the twin nodes are already initialized
        */
        const bool get_twin_init();

        /* 
            Alter the graph
            NOTE: add / delte node: not yet implemented
            NOTE: add edge: nr. edges can not be larger than original nr. edges for a node
        */
        void add_node();
        void delete_node();

        int add_edge_dir(const int src, const int trgt);
        int add_edge(const int src, const int trgt);
        void delete_edge_dir(const int src, const int trgt);
        void delete_edge(const int src, const int trgt);

        std::vector< std::pair<std::pair<int, int>, int> > get_updated_edges();


        /*
            Get all nodes and edges in the graph
        */
        std::set<int> get_nodes();
        std::vector< std::pair<int, int> > get_edges();
        
        /*
            Get degree of node v
        */
        const int get_degree(const int v);

        /*
            Get neighbors of node v
        */
        const std::set<int> get_neighbors(const int v);

        /*
            Get Graph or sparsegraph object representing the d-neighborhood of 
            node v. For directed, both ingoing and outgoing edges are taken into
            account. Nodes included are found by traversing d edges, 
            regardless of directionality.

            Note: New value of v is the position of v in Graph
        */
        Graph get_neighborhood(int &v, const int d);
        void get_neighborhood_nauty(sparsegraph *new_graph, int &v, const int d);
        

        /*
            Get a set containing all nodes in the d-neighborhood of node v.
            Use directed version for directed graph, this accounts for nodes found by
            traversing both the ingoing and outgoing edges

            Note: After execution edges contains the number of edges in the d-neighborhood.
        */
        const std::unordered_set<int> get_neighborhood_nodes(const int v, const int d, int &edges);
        const std::unordered_set<int> get_neighborhood_nodes_directed(const int v, const int d, int &edges);

        /* 
            Get the degree distribution of the graph (out-degree distribution for directed graphs)
            Output: map containing for each degree how often it occurs
        */
        const std::map<int, int>  get_degree_distribution();
        const std::map<int, int> get_degree_distribution_directed();

        /*
            Get set of nodes in giant component
        */
        const std::set<int> get_giant_component_nodes();
        
        /*
            Filter nodelist to only contain nodes in giant component
        */
        void filter_giant_component();

        /*
            Generate a hash representing sparsegrpaph sg
            Output: hash with data type long
        */
        const long get_hash();

        /* 
            Function to print sparse graph. Format:
            from: to, to,
            0: 1, 2, 
            1: 2, 3,
            ...
            Input: sg, sparsegraph to print
        */
        const void print_graph();
};

#endif
