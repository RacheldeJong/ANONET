#ifndef FUZZY
#define FUZZY

#include "../equivalence/equivalence.h"
#include "../measures/measure.h"
#include "../equivalence/cascade.h"
#include "../ged/ged.h"
#include "../util.h"

class FuzzyEP{

    private:
        /*
            Graph 
        */
        Graph *g;

        /*
            Equivalence partition based on which teh GED matrix is computed.
            This also includes the measure used to compute the partition
        */
        EquivalencePartition *ep;

        /*
            GED matrix based on g and ep
        */
        Ged *ged;

        /*
            Fuzzy equivalence partition. Maps each node
            to the set of nodes to which it is equivalent
        */
        std::map<int, std::set<int> > fuzzy_ep;
        
        /*
            Fuzzy anonymity distribution. Counts for each anonymity
            value how many times it occurs
        */
        std::map<int, int> fuzzy_anon_dist;

        /*
            Runtime to compute fuzzy anonymity
        */
        float runtime;

        int max_diff;
        bool perc;
        bool use_twins;

    public:
        /*
            Constructor takes as input the graph and equivalence partition based
            on which the GED matrix is computed.
        */
        FuzzyEP(Graph *g_, EquivalencePartition *ep_, Ged *ged_, int max_diff_) : 
        g(g_), max_diff(max_diff_), ep(ep_), ged(ged_), perc(true), use_twins(true){
            compute_fuzzy_ep(max_diff_, true);
            compute_fuzzy_anon_dist();
        };

        FuzzyEP(Graph *g_, EquivalencePartition *ep_, Ged *ged_, int max_diff_, bool perc_, bool use_twins) : 
        g(g_), max_diff(max_diff_), ep(ep_), ged(ged_), perc(true), use_twins(true){
            compute_fuzzy_ep(max_diff_, perc_);
            compute_fuzzy_anon_dist();
        };

        /*
            Compute the fuzzy anonymity distribution. Returns map that indicates size, and which nodes
            are in an equivalence class of this size. max_diff can be used to set the maximum difference
            for two nodes to be equivalent. use_twins can be set to true to exclude twin nodes in the 
            resulting distribution
        */
        void compute_fuzzy_ep(const int max_diff_, const bool perc_, const bool use_twins=false);
        
        /*
            Compute fuzzy anonymity distribution based no fuzzy_ep 
        */
        void compute_fuzzy_anon_dist();

        /*
            Get and print values
        */
        const std::map<int, std::set<int>> get_fuzzy_ep();
        const std::map<int, int> get_fuzzy_anonymity_distribution();
        const void print_node_to_equivalent();
        const void print_fuzzy_distribution();

        /*
         Returns a set of nodes such that they are at most k-anonymous.
        */
        const double get_frac_non_k_anonymous_nodes(const int k, const bool use_twin_uniqueness);
        const std::set<int> get_non_k_anonymous_nodes(const int k, const bool use_twin_uniqueness);

        /*
            Print statistics: runtime to compute fuzzy k-anonymity
        */
        const void print_statistics();

        const void print_info();

};

#endif
