//
// Created by andrea on 09/09/18.
//
#include <iostream>
#include <algorithm>
#include "lift_to_front_graph.h"

void LiftToFrontGraph::printCurrentStatus() {
    BaseGraph::printCurrentStatus();
    if (this->verbose) {
        std::cout << "  L = { ";
        for (int &index : this->list) {
            std::cout << index << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void LiftToFrontGraph::preProcess(int s, int t) {
    BaseGraph::preProcess(s, t);
    for (int i = 0; i < this->vertices.size(); i++) {
        if (i != s && i != t) {
            this->list.push_back(i);
        }
    }
}

int LiftToFrontGraph::getActiveNode(int s, int t) {
    for (int index : this->list) {
        if (this->vertices[index].excess > 0) {
            return index;
        }
    }
    return NO_ACTIVE_NODE_FOUND;
}

bool LiftToFrontGraph::relabel(int u) {
    bool relabeled = BaseGraph::relabel(u);
    if (relabeled && this->list[0] != u) {
        // the vertex u must be placed in front of the list L
        auto it = std::find(this->list.begin(), this->list.end(), u);
        std::rotate(this->list.begin(), it, it + 1);
    }
    return relabeled;
}