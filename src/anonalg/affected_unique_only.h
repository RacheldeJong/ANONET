#ifndef ANON_AFF_UNIQUE_ONLY
#define ANON_AFF_UNIQUE_ONLY

#include "anonymizationalgorithm.h"

#define VARIANT_AFFUNIQUE_U 0
#define VARIANT_AFFUNIQUE_U_NORM 1
#define VARIANT_AFFUNIQUE_UU 2
#define VARIANT_AFFUNIQUE_UU_NORM 3

class AnonAffectedUniqueOnly : public Anonymizationalgorithm{
    
    private:
        std::vector<int> variants = {VARIANT_AFFUNIQUE_U, VARIANT_AFFUNIQUE_U_NORM,
                    VARIANT_AFFUNIQUE_UU, VARIANT_AFFUNIQUE_UU_NORM};
    
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