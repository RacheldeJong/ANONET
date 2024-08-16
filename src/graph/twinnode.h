// twinnode.h: 
//
// Contains code to find twin nodes and keep track of the twinnode map
//
// Last edited: dd-mm-2023
//

#ifndef TWINNODE
#define TWINNODE

#include "../util.h"
#include "nauty_graph.h"
#define EMPTYSIZE 0;

#define MAX_DEG INT_MAX

class Graph;

class Twinnodes{

   private:
      std::vector< std::vector< int > > twinnode_map;
      std::vector< int > node_to_twin;
      std::vector< int > twin_minimal;
      Graph* graph;
      int max_nbs_twin;

      void compute_twinnodes();
      void compute_minimal_twins();
      void compute_node_to_twin();
      void compute_nr_twins();
   
      bool initialized;

      int twinnode_count;
      int non_twinnode_count;

   public:

      void compute();

      /* 
         Constructor
      */
      Twinnodes(Graph* g_, const int max_nbs_twin_=-1) : graph(g_){
         if(max_nbs_twin_ == -1) max_nbs_twin = MAX_DEG;
         else max_nbs_twin = max_nbs_twin_;
         initialized = false;
      };

      /* 
         Update twinnode map
      */
      void update_twinnodes(std::vector<int> changed_nodes);
      
      /*
         Return vectors such that nodes in vec are twins
      */
      const std::vector<std::vector<int>> get_twinnodes();

      /*
         Get vector containing one node for each set of twin nodes
      */
      const std::vector<int> get_minimal_twins();
      const std::vector<int> get_node_to_twin();

      const bool is_initialized();

      /*
         Statistics
      */
      const int get_nr_twinnodes();
      const int get_nr_nontwinnodes();

      const void print_twinnodes();
};

#endif