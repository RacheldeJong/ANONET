#ifndef ANONYMIZATION_H
#define ANONYMIZATION_H

#include "anonymizationalgorithm.h"
#include "randomalg.h"
#include "unique_only.h"
#include "degree.h"
#include "affected_total.h"
#include "affected_unique.h"
#include "affected_unique_only.h"
#include "../util.h"

class Anonymization{

    private:
        /*
            Graph to anonymize
        */
        Graph *g;
        /*
            EP corresponding to g
        */
        EquivalencePartition *EP;
        /*
            Anonymization algorithm chosen. Options:
            - Random
        */
        int anon_alg_nr;
        /*
            Number of operations to perform by the algorithm.
            Currently: edge deletions
        */
        int budget;
        /*
            After how many edge deletions the EP and datastructures
            used for anonymization algorithms should be recomputed
        */
        int recompute_gap;

        std::vector<double> uniqueness;
        std::vector<double> twin_uniqueness;
        std::vector<double> runtimes;
        int k;

    public:
        Anonymization(Graph *g_, EquivalencePartition *EP_, int anon_alg_nr_, const int budget_, const int recompute_gap_, const int k_=2) :
        g (g_), EP(EP_), budget(budget_), recompute_gap(recompute_gap_), anon_alg_nr(anon_alg_nr_){
            anonalg = Anonymizationalgorithm();
            k = 2;
            
            if(anon_alg_nr == ALG_RAND){
                Anonrandom *alg = new Anonrandom();
                anonalg.setMeasure(alg);
            }
            else if(anon_alg_nr == ALG_UNIQUE){
                AnonUnique *alg = new AnonUnique();
                anonalg.setMeasure(alg);
            }
            else if(anon_alg_nr == ALG_DEGREE){
                AnonDegree *alg = new AnonDegree();
                anonalg.setMeasure(alg);
            }
            else if(anon_alg_nr == ALG_AFF_TOTAL){
                AnonAffectedTotal *alg = new AnonAffectedTotal();
                anonalg.setMeasure(alg);
            }
            else if(anon_alg_nr == ALG_AFF_UNIQUE){
                AnonAffectedUnique *alg = new AnonAffectedUnique();
                anonalg.setMeasure(alg);
            }
            else if(anon_alg_nr == ALG_AFF_UNIQUE_ONLY){
                AnonAffectedUniqueOnly *alg = new AnonAffectedUniqueOnly();
                anonalg.setMeasure(alg);
            }
            /*
            else if(anon_alg_nr == ...){
                ... *alg = new ...();
                anonalg.setMeasure(alg);
            }
            */
            else{
                Anonrandom *alg = new Anonrandom();
                anonalg.setMeasure(alg);
            }
            anonalg.set_k(k);

        };

        ~Anonymization(){
        };

        Anonymizationalgorithm anonalg;

        /*
            Perform the anonymization (called in constructor)
        */
        void anonymize();

        /*
            Print info and statistics about the anonymization process
        */
        void print_info();
        void print_statistics();

        void set_k(const int k_);
        void set_alg_variant(const int variant);
};

#endif