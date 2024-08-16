#ifndef ANON_AFF_UNIQUE
#define ANON_AFF_UNIQUE

#include "anonymizationalgorithm.h"

#define VARIANT_AFFUNIQUE 0
#define VARIANT_AFFUNIQUE_NORM 1

class AnonAffectedUnique : public Anonymizationalgorithm{
    
    private:
        std::vector<int> variants = {VARIANT_AFFUNIQUE, VARIANT_AFFUNIQUE_NORM};
    
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