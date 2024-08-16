/* Contains functions required to implement the measure / definition for equivalence
 * and get the affected nodes
*/

#ifndef COUNT_H
#define COUNT_H

#define N_POS_CACHE 0
#define M_POS_CACHE 1

#include "measure.h"

class Count : public Measure{

    private:

        /*
            Distance for measure: Radius of d-neighborhood for which
            degree distribution is computed
        */
        int distance = 1;

        sparsegraph *subgraph;

        void init(Graph *g){
            int n = g->get_number_nodes();
            int m = g->get_number_edges();
            subgraph = new sparsegraph;
            SG_INIT(*subgraph);
            SG_ALLOC(*subgraph, n, m, "malloc");
        }

    public:

        /*
            Constructor. Distance is optional, if not given set to 1
        */
        Count(Graph *g){
            init(g);
        };
        Count(Graph *g, int d_): distance(d_){
            init(g);
        };
        ~Count(){
            SG_FREE(*subgraph);
            delete subgraph;
        };
        
        /*
            Compute the value for node v
        */
        const cache_pair compute_value(Graph* g, const int v);

        /*
            Determine if values for two nodes are equivalent
        */
        const bool are_equivalent(Graph* g, const int v1, const int v2);
        
        /*
            Compute editing distance between nodes for the measure
        */
        const long compute_difference(Graph* g, const int v1, const int v2);
        const long compute_difference(Graph* g, cache_pair val1, cache_pair val2);
        
        /*
            Compute the set of nodes that is affected if edge (source, target)
            is added or deleted
        */
        const std::set<int> get_nodes_affected(Graph* g, const int source, const int target);

        /*
            Print the value. Note that types of values differ per measure
        */
        const void print_val_measure(const cache_pair val);
        
        /*
            Determine if the given values are equal to each other.
            Note that types of values differ per measure
        */
        const bool are_equal(const cache_pair, const cache_pair val2);
};

#endif