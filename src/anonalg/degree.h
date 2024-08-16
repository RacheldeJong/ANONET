#ifndef ANON_DEGRE
#define ANON_DEGREE

#define VARIANT_MIN 0
#define VARIANT_MAX 1
#define VARIANT_SUM 2
#define VARIANT_DIFF 3


#include "anonymizationalgorithm.h"

class AnonDegree : public Anonymizationalgorithm{
    

    private:

      std::vector<int> variants = {VARIANT_MIN, VARIANT_MAX, VARIANT_SUM, VARIANT_DIFF};
    
    public:
        /*
            Select edges
        */
        std::vector< std::pair<int, int> > select_edges(Graph *g, 
            EquivalencePartition *EP, const int nr_edges);
        
        void update(Graph *g, EquivalencePartition *EP){};

        std::vector<int> get_variants(){return variants;};

};

#endif