/* 
    Measure interface. When implementing a new measure class, please
    implement the following functions in the class. Examples can be found in the 
    different measure implementations.
    Additionally, create a measure ID here, and a default heuristic
*/

#ifndef ANONYMIZATIONALG_H
#define ANONYMIZATIONALG_H

// Create id for the measure
#define ALG_RAND 0
#define ALG_UNIQUE 1
#define ALG_DEGREE 2
#define ALG_AFF_TOTAL 3
#define ALG_AFF_UNIQUE 4
#define ALG_AFF_UNIQUE_ONLY 5

#define VARIANT_DEFAULT 0

#include "../util.h"
#include "../graph/nauty_graph.h"
#include "../equivalence/equivalence.h"

void print_alg(const int meas_choice, const int variant=0, char end='\n');

class AnonymizationAlgorithmInterface {
    
    public:

        int k;

        int variant;
        std::vector<int> variants;


        virtual const std::vector< std::pair<int, int> > select_edges(Graph *g, EquivalencePartition *EP, const int nr_edges){
            return g->get_edges();
        };

        virtual const void set_k(const int k_){};

        virtual const void set_variant(const int variant_){};

        virtual const void update(Graph *g, EquivalencePartition EP){};
        
        virtual const std::vector<int> get_variants(){return variants;};

        virtual ~AnonymizationAlgorithmInterface() {};
};

class Anonymizationalgorithm {


    private:
        std::vector<int> variants = {VARIANT_DEFAULT};
    
    public:
        Anonymizationalgorithm *anonalg;

        int variant;
        int k;

        /*
            Set the algorithm
        */
        void setMeasure(Anonymizationalgorithm *anonalg_){
            anonalg = std::move(anonalg_);
        };

        virtual void set_k(const int k_){anonalg->k = k_;};

        /*
            Set variant of the anonymization algorithm
        */
        virtual void set_variant(const int variant_){
            anonalg->variant = variant_;
        };

        /*
            Select the edges
        */
        virtual std::vector< std::pair<int, int> > select_edges(Graph *g, EquivalencePartition *EP, const int nr_edges){
            return anonalg->select_edges(g, EP, nr_edges);
        };

        /*
            Update the datastructures if necessairy
        */
        virtual void update(Graph *g, EquivalencePartition *EP){
            anonalg->update(g, EP);
        };

        /*
            Get variants for the chosen measure in a vector
        */
        virtual std::vector<int> get_variants(){
            return anonalg->get_variants();
        };

};

#endif
