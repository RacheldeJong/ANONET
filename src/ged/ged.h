#include "../equivalence/equivalence.h"
#include "../measures/measure.h"
#include "../equivalence/cascade.h"
#include "../util.h"
#include "../graph/nauty_graph.h"

#ifndef GED
#define GED

class Ged{

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
        std::vector< std::vector<int> > ged_matrix;

    public:
        /*
            Constructor takes as input the graph and equivalence partition based
            on which the GED matrix is computed.
        */
        Ged(Graph *g_, EquivalencePartition *ep_) : g(g_), ep(ep_){
            compute_ged_matrix();
        };

        /*
            Compute the GED matrix
        */
        void compute_ged_matrix();

        /*
            Get ged value in cell ged_matrix[i][j]
        */
        const int get_ged_value(const int i, const int j);
        
        /*
            Returns a copy of the GED matrix
        */
        const std::vector<std::vector<int>> get_ged_matrix();

        /*
            Print the GED matrix
        */
        const void print_ged_matrix();

};

#endif
