/* 
   Class responsible for keeping track of equivalence partition.
   Equivalence partition, consisting of equivalence classes, are created 
   for a graph based on the given measure. After altering graph, equivalence 
   classes can be updated by passing the nodes for which it might be affected
*/

#ifndef EQUIVALENCE_H
#define EQUIVALENCE_H

#include "../util.h"
#include "cache.h"
#include "../graph/nauty_graph.h"
#include "../graph/twinnode.h"

class EquivalencePartition{
   private:
        
      /* 
         Graph for which the EP should be computed
      */
      Graph* g;
  
      /* 
         Measure choices: measure, heuristic, distance, and iterative
      */
      int meas_choice;
      int heur_choice;
      int max_dist;
      bool iterative_computation;
      bool twin_heuristic;

      /* 
         Equivalence partition for chosen measure
      */
      std::vector<std::set<int>> epartition;

      /* 
         Initial EP passed in constructor: used only if measure requires
         multiple iterations
      */
      std::vector< std::set<int> > ep_initial;

      /* 
         EP objects containing results for measure or heuristic with
         dist [1, 2, ..., max_dist]. Used if measure requires multiple
         iterations, or if a heuristic is used
      */
      std::vector<EquivalencePartition*> heuristicEPS;
      std::vector<EquivalencePartition*> distEPs;

      /* 
         Map node to corresponding equivalence class
      */
      std::vector<int> node_to_ep_entry;

      /* 
         Help function to split a single equivalence class using the measure
      */
      std::vector< std::set<int> > split_eclass(std::set<int> eclass);
        
      /* 
         Add and remove twin nodes to equivalence partition
      */
      std::vector< std::set<int>> remove_twins(std::vector<std::set<int>> ep_cur);
      std::vector< std::set<int>> add_twins(std::vector<std::set<int>> ep_cur);

      /* 
         Compute EP and stores in EP
      */
      void compute_equivalence_partition();

      /* 
        Compute vector that maps each node to the corresponding equivalence class
      */
      void compute_node_to_ec();

      /*
         Store runtimes
      */
      std::vector<float> runtimes;
      float runtime_tot;

      /* 
         Help function for update
      */
      void update_nodes(std::set<int> nodes);
      std::set<int> update_node(const int node, std::set<int> ec_cur);
      void remove_nodes_from_ep(std::set<int> nodes);
      void remove_empty_ecs();
      /*
         Counts for how many edges has been updated. For the update
         therafter, only nodes have to be updated based on newly updated 
         edges.
      */
      int update_count;

      /* 
         Constructor: using initial EP computed internally
         NOTE: USED IN CLASS ONLY
      */
      EquivalencePartition(Graph* g_, std::vector< std::set<int> > ep_initial_, std::pair<int, int> meas_choice_, int heur_choice_) 
      : ep_initial(ep_initial_), cache(g_, meas_choice_), g(g_){
         max_dist = 1;
         meas_choice = meas_choice_.first;
         heur_choice = heur_choice_;
         iterative_computation = false;
         twin_heuristic = false;
         update_count  = 0;
         compute_equivalence_partition();
      };

      EquivalencePartition(Graph* g_, std::vector< std::set<int> > ep_initial_, int meas_choice_, int heur_choice_) 
      : ep_initial(ep_initial_), cache(g_, meas_choice_), g(g_){
         max_dist = 1;
         meas_choice = meas_choice_;
         heur_choice = heur_choice_;
         iterative_computation = false;
         twin_heuristic = false;
         update_count = 0;

         compute_equivalence_partition();
      };

   public:
      /* 
         Constructor
      */
      EquivalencePartition(Graph* g_, int meas_choice_ = 0, int heur_choice_=0) : cache(g_, meas_choice_), g(g_),
      meas_choice(meas_choice_), heur_choice(heur_choice_){
         max_dist = 1;
           
         // Default: Heuristic choice depends on measure choice
         ep_initial = {};
         iterative_computation = true;
         twin_heuristic = false;
         update_count = 0;

         compute_equivalence_partition();
      };

      EquivalencePartition(Graph* g_, std::pair<int, int> meas_choice_, int heur_choice_=0) : 
      cache(g_, meas_choice_), g(g_), meas_choice(meas_choice_.first), max_dist(meas_choice_.second),
      heur_choice(heur_choice_){
         meas_choice = meas_choice_.first;
         max_dist = meas_choice_.second;

         // Default: Heuristic choice depends on measure choice
         ep_initial = {};
         iterative_computation = true;
         twin_heuristic = true;
         update_count = 0;

         compute_equivalence_partition();
      };

      ~EquivalencePartition(){
         heuristicEPS.clear();
         distEPs.clear();
      };

      /* 
         Cache stores all computed values for nodes
         public: to get statistics
      */
      Cache cache;

      /*
         Update Equivalence partition based on updated edges of g
      */
      void update_ep();

        
      /* 
         Returns EP for the graph and measure given to constructor
      */
      std::vector< std::set<int> > get_equivalence_partition();

      /* 
         Get or print equivalence distribution i.e., the size 
         distribution of equivalence classes
      */
      const std::map<int, int> get_equivalence_distribution(std::vector<std::set<int>>epartition_cur);
      const std::map<int, int> get_equivalence_distribution();

      /* 
         Get equivalence class id of given node
      */
      int get_eq_entry(const int v);

      /* 
         Get EPs for the heuristics used and the different distances, if iterative approach is used
      */
      std::vector< std::set<int> > get_heuristic_partition(std::vector< std::set<int> > eq_cur, const int d);
      std::vector< std::set <int > > get_distance_partition(std::vector< std::set<int> > initial_partition, const int cur_dist);
      EquivalencePartition* get_heuristic_partition(const int cur_dist);
      EquivalencePartition* get_distance_partition(const int cur_dist);

      /* 
         Get a set of representative nodes: one node for each equivalence class
         Vector returned contains a set of nodes, such that there is exactly
         one node for each equivalence class
      */
      const std::vector<int> get_representative_nodes();

      /*
         Returns vector mapping each node to their equivalence class entry in EP.
         EP can be obtained by get_equivalence_partition().
      */
      const std::vector<int> get_node_to_ep_entry();
      
      /*
         Get vector with for each node its anonymity (value for k)
      */
      const std::vector<int> get_node_to_anonymity(bool minustwin=false);

      /*
         Returns a set of nodes such that they are at most k-anonymous.
      */
      const std::set<int> get_k_anonymous_nodes(const int k, const bool minustwin);
      const double get_frac_k_anonymous_nodes(const int k, const bool minustwin);

      /*
         Print functions:
         Print_equivalence_partition: print for each ec, which nodes occur in it.
         Print_equivalence_distribution: print for each equivalence class size often it occurs
            If use_twin=false, the non-representative twin nodes are removed from the equivalence partition
            before determining the size.
            NOTE: when use_twin=false, nodes represented by the equivalence classes in this case does not equal the total number
            of nodes.
         Print_node_anonymity: print for each node its anonymity.
            If use_twin=false, the non-representative twin nodes are removed from the equivalence partition
            before determining the size. This influences the anonymity (k value) of nodes
      */
      const void print_equivalence_partition();
      const void print_equivalence_distribution(bool minustwin=false);
      const void print_node_anonymity(bool minustwin=false);

      /*
         Print runtimes: print runtime per iteration, and total runtime
      */
      const void print_runtimes();

      /*
         Get and print anonymity distribution. This indicates how many nodes are at most k-anonymous.
         If use_twin=false, the non-representative twin nodes are removed from the equivalence partition
            before determining the size. This influences the anonymity (k value) of nodes
      */
      const void print_anonymity_distribution(bool minustwin=false);
      const std::map<int, int> get_anonymity_distribution(bool minustwin=true);

      /*
         Print information about setting used
      */
      const void print_info();
      
      /*
         Set and get settings
      */
      void set_twin_heuristic(const bool value);
      void set_iterative(const bool value);
      const bool get_twin_heuristic();
      const bool get_iterative();
};

#endif