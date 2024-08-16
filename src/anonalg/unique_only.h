#ifndef ANON_UNIQUE
#define ANON_UNIQUE

#include "anonymizationalgorithm.h"

#define VARIANT_U 0
#define VARIANT_UU 1

class AnonUnique : public Anonymizationalgorithm{
    
    private:
        int k;
        std::vector<int> variants = {VARIANT_U, VARIANT_UU};
    
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