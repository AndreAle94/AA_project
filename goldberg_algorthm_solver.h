//
// Created by andrea on 09/09/18.
//

#ifndef ADVANCEDALGORITHMSPROJECT_GOLDBERG_ALGORTHM_SOLVER_H
#define ADVANCEDALGORITHMSPROJECT_GOLDBERG_ALGORTHM_SOLVER_H

#include "base_graph.h"

enum SolverType {
    GENERIC_SOLVER,
    LIFT_TO_FRONT_SOLVER
};

class GoldbergProblemSolver {

    BaseGraph* graph;

public:

    GoldbergProblemSolver(int vertices_count, SolverType type, bool verbose);

    virtual ~GoldbergProblemSolver();

    void addEdge(int u, int v, int capacity);

    int getMaximumFlow(int s, int t);
};

#endif //ADVANCEDALGORITHMSPROJECT_GOLDBERG_ALGORTHM_SOLVER_H