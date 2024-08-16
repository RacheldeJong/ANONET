/* 
    Measure interface. When implementing a new measure class, please
    implement the following functions in the class. Examples can be found in the 
    different measure implementations.
    Additionally, create a measure ID here, and a default heuristic
*/

#ifndef MEASURE_H
#define MEASURE_H

// Create id for the measure
#define MEAS_NONE 0
#define MEAS_DEGREE 1
#define MEAS_COUNT 2
#define MEAS_DEGDIST 3
#define MEAS_DKANON 4
#define MEAS_HAY 5
#define MEAS_HYBRID 6
#define MEAS_EDGECOUNT 7

// Set default heuristic for measure
#define HEUR_NONE MEAS_NONE
#define HEUR_DEGREE MEAS_NONE
#define HEUR_COUNT MEAS_DEGREE
#define HEUR_DEGDIST MEAS_COUNT 
#define HEUR_DKANON MEAS_COUNT
#define HEUR_HAY MEAS_DEGREE
#define HEUR_HYBRID MEAS_HAY
#define HEUR_EDGECOUNT MEAS_NONE

#include "../util.h"
#include "../graph/nauty_graph.h"

void print_measure(const int meas_choice, char end='\n');

class MeasureInterface {
    public:

        virtual const cache_pair compute_value(const Graph* g, const int v){
            std::map<int, int> empty_map = {};
            int empty_val = -1;
            return std::make_pair(empty_val, empty_map);
        };

        virtual const bool are_equivalent(Graph* g, const int v1, int v2){
            return true;
        };

        virtual const long compute_difference(Graph* g, const int v1, const int v2){
            return 0;
        };

        virtual const long compute_difference(Graph* g, cache_pair val1, cache_pair val2){
            return 0;
        };

        virtual const std::set<int> get_nodes_affected(Graph* g, const int source, const int target){
            std::set<int> nodes; return nodes;
        };

        virtual ~MeasureInterface() {};
};

class Measure {

    Measure *meas;

    private:

        /*
            Contains how far the measure looks: default 1
            Note: not taken into account for Degree
        */
        int d = 1;
    
    public:

        /*
            Set the measure (for this interface class only)
        */
        void setMeasure(Measure *strat_){
            meas = std::move(strat_);
        };

        /*
            Compute value for node v in graph g
        */
        virtual const cache_pair compute_value(Graph* g, const int v){
            return meas->compute_value(g, v);
        }; 
        
        /*
            Determine if values for two nodes are equivalent
        */
        virtual const bool are_equivalent(Graph* g, const int v1, const int v2){
            return meas->are_equivalent(g, v1, v2);
        };

        /*
            Compute editing distance between nodes for the measure
        */
        virtual const long compute_difference(Graph* g, const int v1, const int v2){
            return meas->compute_difference(g, v1, v2);
        };
        virtual const long compute_difference(Graph* g, cache_pair val1, cache_pair val2){
            return meas->compute_difference(g, val1, val2);
        };
        
        /*
            Compute the set of nodes that is affected if edge (source, target)
            is added or deleted
        */
        virtual const std::set<int> get_nodes_affected(Graph* g, const int source, const int target){
            return meas->get_nodes_affected(g, source, target);
        };
};

#endif
