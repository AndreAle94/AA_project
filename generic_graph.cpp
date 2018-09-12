//
// Created by andrea on 09/09/18.
//
#include "generic_graph.h"

int GenericGraph::getActiveNode(int s, int t) {
    for (int i = 0; i < this->vertices.size(); i++) {
        if (i != s && i != t && this->vertices[i].excess > 0) {
            return i;
        }
    }
    return NO_ACTIVE_NODE_FOUND;
}