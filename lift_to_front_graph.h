//
// Created by andrea on 09/09/18.
//

#ifndef ADVANCEDALGORITHMSPROJECT_LIFT_TO_FRONT_GRAPH_H
#define ADVANCEDALGORITHMSPROJECT_LIFT_TO_FRONT_GRAPH_H

#include "base_graph.h"

class LiftToFrontGraph : public BaseGraph {

    std::vector<int> list;

public:

    explicit LiftToFrontGraph(int vertices_count) : BaseGraph(vertices_count) {}

    void printCurrentStatus() override;

    void preProcess(int s, int t) override;

    int getActiveNode(int s, int t) override;

    bool relabel(int u) override;
};

#endif //ADVANCEDALGORITHMSPROJECT_LIFT_TO_FRONT_GRAPH_H