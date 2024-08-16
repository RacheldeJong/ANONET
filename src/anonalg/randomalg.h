#ifndef ANON_RAND
#define ANON_RAND

#include "anonymizationalgorithm.h"

class Anonrandom : public Anonymizationalgorithm{
    
    private:
        int k;
        std::vector<int> variants = {VARIANT_DEFAULT};
    
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