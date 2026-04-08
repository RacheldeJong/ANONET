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

    private:    double u_best = 1.0;

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

        /*
            Settings for computing anonymity
            - Toggle using twin-uniqueness
            - If fuzzy_budget >0: use fuzzy k-anonymity
            - If fuzzy_perc: treat budget as perc. otherwise as number difference
        */
        bool use_twin_uniqueness;
        bool use_fuzzy;
        int fuzzy_budget;
        bool fuzzy_perc;

        std::set<int> compute_anonymity(float &uniqueness, const bool fuzzy);

        std::vector<std::pair<int, int>> deleted_edges_all;
        std::vector<int> edges_altered;
        std::vector<double> perc_edges_altered;
        std::vector<double> uniqueness;
        std::vector<double> twin_uniqueness;
        std::vector<double> fuzzy_uniqueness;
        std::vector<double> runtimes;
        std::vector<double> runtimes_alg;
        std::vector<double> runtimes_update;
        int k;

    public:
        Anonymization(Graph *g_, EquivalencePartition *EP_, int anon_alg_nr_, const int budget_, const int recompute_gap_, const int k_=2) :
        g (g_), EP(EP_), budget(budget_), recompute_gap(recompute_gap_), anon_alg_nr(anon_alg_nr_){
            anonalg = Anonymizationalgorithm();
            k = k_;
            use_twin_uniqueness = false;
            use_fuzzy = false;
            fuzzy_budget = false;
            fuzzy_perc = false;
            
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
        const void print_info();
        const void print_statistics();
        const void print_altered_edges();

        void set_k(const int k_);
        void set_alg_variant(const int variant);
        void set_twin_uniqueness(const bool twin_uniqueness);
        void set_fuzzy_anon(const int fuzzy_budget_, const bool fuzzy_perc_, const bool use_fuzzy_);
};

#endif