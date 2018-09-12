//
// Created by andrea on 09/09/18.
//

#ifndef ADVANCEDALGORITHMSPROJECT_GENERICGRAPH_H
#define ADVANCEDALGORITHMSPROJECT_GENERICGRAPH_H

#include "base_graph.h"

class GenericGraph : public BaseGraph {

public:

    explicit GenericGraph(int vertices_count) : BaseGraph(vertices_count) {}

    int getActiveNode(int s, int t) override;
};

#endif //ADVANCEDALGORITHMSPROJECT_GENERICGRAPH_H