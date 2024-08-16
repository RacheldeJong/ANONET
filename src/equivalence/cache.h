/* This part implements the cache which stores for each node the
 * obtained value for a measure
*/

#ifndef CACHE_H
#define CACHE_H

#include "../util.h"
#include "../graph/nauty_graph.h"

// List of measures
#include "../measures/degdist.h"
#include "../measures/hay.h"
#include "../measures/dkanonymity.h"
#include "../measures/count.h"
#include "../measures/none.h"
#include "../measures/degree.h"
#include "../measures/hybrid.h"
#include "../measures/edgecount.h"

#define CACHE_TYPE_INT 0
#define CACHE_TYPE_MAP 1
#define DEFAULT_DIST 1
const std::map<int, int> EMPTY_MAP = {};
const cache_pair EMPTY_CACHE_PAIR = std::make_pair(-2, EMPTY_MAP);

class Cache{
    
    private:

        /* 
            Stores for each node the corresponding value
        */
        std::map<int, cache_pair> cache;
        int cache_type;

        /*
            Cache statistics
        */
        int vals_computed;
        int vals_from_cache;

        /*
            The measure classes set in the constructur. Contains functions
            to determine cache values and equivalence.
        */

        /*
            Graph for which cache values are computed
        */
        Graph *g;

        /*
            Checks if given values are equal. Type dependent on measure used
        */
        const bool are_equal(const cache_pair val1, const cache_pair val2);

        /*
            Prints value. Type dependent on measure used
        */
        const void print_val_measure(const cache_pair val);
        const void print_val_measure(const int val);
        const void print_val_measure(const std::map<int, int> val);

        void init(const int meas, const int dist){
            eq_measure = Measure();
            vals_computed = 0;
            vals_from_cache = 0;

            if(meas == MEAS_DEGREE){
                Degree *m = new Degree(g);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_INT;
            }
            else if(meas == MEAS_DEGDIST){
                Degdist *m = new Degdist(g, dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_MAP;
            }
            else if(meas == MEAS_HAY){
                Hay *m = new Hay(dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_MAP;
            }
            else if(meas == MEAS_COUNT){
                Count *m = new Count(g, dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_MAP;
            }
            else if(meas == MEAS_DKANON){
                dkAnonymity *m = new dkAnonymity(g, dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_INT;
            }
            else if(meas == MEAS_HYBRID){
                Hybrid *m = new Hybrid(g, dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_INT;
            }
            else if(meas == MEAS_EDGECOUNT){
                Edgecount *m = new Edgecount(g, dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_INT;
            }
            else{
                None *m = new None(dist);
                eq_measure.setMeasure(m);
                cache_type = CACHE_TYPE_INT;
            }
        }

    public:
        
        /*
            Measure used
        */
        Measure eq_measure;


        /*  
            Constructur. Create empty cache and set the measure.
            Variants:
            - Naive
            - Iterative
        */
        Cache(Graph* g_, const int measure_choice = 0) : g(g_){
            init(measure_choice, DEFAULT_DIST);
            
        };

        Cache(Graph* g_, const std::pair<int, int> measure_choice) : g(g_){
            init(measure_choice.first, measure_choice.second);
        };

        /*
            Destructor
        */
        ~Cache();

        /*
            Returns the cache value for node v
        */
        cache_pair get_cache_entry(const int v);

        /*
            Returns the cache value for node v
            only if computed. Otherwise return
            empty cache pair
        */
        cache_pair get_cache_entry_computed(const int v);

        /*  
            Return the number of elements in the cache
        */
        int get_cache_size();

        /* 
            Update value for node v
        */
        void update_cache_entry(const int v);

        /*
            Returns if nodes are equivalent w.r.t. eq_measure
        */
        const bool are_equivalent(const int v1, const int v2);

        /*
            Print value for a given node
        */
        const void print_val_node(const int v);

        /*
            Return number of values computed
        */
        const int get_vals_computed();

        /*
            Print statistics about cache use: size, nr. vals computed,
            nr. vals obtained from cahe
        */
        const void print_statistics();
};

#endif