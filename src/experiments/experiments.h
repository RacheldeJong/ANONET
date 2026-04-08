
#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include "../graph/nauty_graph.h"
#include "../graph/graphgen.h"
#include "../equivalence/equivalence.h"
#include "../util.h"
#include "../tests/test.h"
#include "../anonalg/anonymization.h"
#include "../equivalence/cascade.h"

#define EXP_ANON 0
#define EXP_CASCADE 1
#define EXP_FUZZY 2
#define EXP_EXAMPLE 3

extern int statistics;

void do_experiment(const int choice, char *input_file, sparsegraph sg1);

void anon_experiment(char *input_file, sparsegraph sg1);
void exp_fuzzy_anonymization(char *input_file, sparsegraph sg1);
void example_experiment(char *input_file, sparsegraph sg1);
#endif