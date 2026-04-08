
#ifndef CASCADE_H
#define CASCADE_H

#include "equivalence.h"
#define K_UNIQUE 1

class AnonymityCascade{
    
    private:
        
        /*
            Helper functions for Anonymity-cascade algorithm
        */
        std::map<int, std::set<int> > get_eq_dist_nodeset(Graph *g, std::vector<int> node_to_ep, std::set<int> nodes);
        std::set<int> get_unique_nodes(Graph *g, std::map< int, std::set<int> > eq, bool twinnodes);

        /*
            Store statistics
            - Runtime
            - Unique nodes after x levels
            - Total number of unique nodes
        */
        float runtime_tot=0.0;
        std::vector<int> unique_level;
        int unique_count=0;

        Graph *g;
        EquivalencePartition *EP1, *EP2;
        int max_level;
        bool twinnodes;

    public:
        AnonymityCascade(Graph *g_, EquivalencePartition *EP1_, EquivalencePartition *EP2_, 
            bool twinnodes_=false, const int level=-1) : g(g_), EP1(EP1_), EP2(EP2_), twinnodes(twinnodes_), max_level(level){
                anonymity_cascade();
            };
        AnonymityCascade(Graph *g_, EquivalencePartition *EP, int level) : g(g_), EP1(EP), EP2(EP), twinnodes(false),
        max_level(level){};
        AnonymityCascade(Graph *g_, EquivalencePartition *EP) : g(g_), EP1(EP), EP2(EP), twinnodes(false),
        max_level(-1){};

        /*
            Perform anonymity-cascade. EP1 is used to find the starting nodes.
            Ep2 is used to determine which nodes to use for the cascading effect.
            Returns a set of nodes that can be uniquely or twin-uniquely 
            identified given the EP(s).

        */
        std::set<int> anonymity_cascade();


        const void print_info();

        /*
            Print statistics:
            - stat=0: number of unique nodes only
            - stat=1: additionally print unique nodes after x levels, for each level
            - stat=2: additionally print runtime
        */
        const void print_stats(const int stat=0);
};

#endif