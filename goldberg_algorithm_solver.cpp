//
// Created by andrea on 09/09/18.
//
#include "goldberg_algorthm_solver.h"
#include "generic_graph.h"
#include "lift_to_front_graph.h"

GoldbergProblemSolver::GoldbergProblemSolver(int vertices_count, SolverType type, bool verbose) {
    switch (type) {
        case GENERIC_SOLVER:
            this->graph = new GenericGraph(vertices_count);
            break;
        case LIFT_TO_FRONT_SOLVER:
            this->graph = new LiftToFrontGraph(vertices_count);
            break;
    }
    this->graph->setVerbose(verbose);
}

GoldbergProblemSolver::~GoldbergProblemSolver() {
    delete this->graph;
}

void GoldbergProblemSolver::addEdge(int u, int v, int capacity) {
    this->graph->addEdge(u, v, capacity);
}

int GoldbergProblemSolver::getMaximumFlow(int s, int t) {
    this->graph->getMaximumFlow(s, t);
}