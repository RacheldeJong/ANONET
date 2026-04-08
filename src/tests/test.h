/*
    Test functions for the ANONET package
*/

#ifndef TEST_H
#define TEST_H


#include "../graph/nauty_graph.h"
#include "../graph/graphgen.h"
#include "../equivalence/equivalence.h"
#include "../util.h"
#include "../equivalence/cache.h"
#include "../graph/twinnode.h"
#include "../measures/dkanonymity.h"

void graphutil_test(sparsegraph sg);
void cache_test(sparsegraph sg);
void equivalence_test(sparsegraph sg);
void twin_test(sparsegraph sg);
void del_add_test(sparsegraph sg);
void heuristic_test(sparsegraph sg);
void update_test(sparsegraph sg);
#endif