//
// Created by andrea on 09/09/18.
//
#include <iostream>
#include "base_graph.h"

/**
 * This constructor generate takes as input the size of the graph and creates an internal structure
 * keeping a reference for each vertex and an incident matrix to know which vertex are connected.
 * @param vertices_count size of the graph.
 */
BaseGraph::BaseGraph(int vertices_count) {
    this->edge_matrix = new Edge**[vertices_count];
    for (int i = 0; i < vertices_count; i++) {
        this->vertices.emplace_back(DEFAULT_LABEL, DEFAULT_EXCESS);
        this->edge_matrix[i] = new Edge*[vertices_count];
    }
    // initialize pointers to null
    for (int i = 0; i < this->vertices.size(); i++) {
        for (int j = 0; j < this->vertices.size(); j++) {
            this->edge_matrix[i][j] = nullptr;
        }
    }
}

BaseGraph::~BaseGraph() {
    for (int i = 0; i < this->vertices.size(); i++) {
        for (int j = 0; j < this->vertices.size(); j++) {
            delete this->edge_matrix[i][j];
        }
        delete this->edge_matrix[i];
    }
    delete this->edge_matrix;
}

void BaseGraph::printCurrentStatus() {
    // print debug info if verbose is enabled
    if (this->verbose) {
        std::cout << "============================================================================ " << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << "  [vertex " << i << "] => label: " << vertices[i].label << ", excess: " << vertices[i].excess << std::endl;
        }
        std::cout << "  [incidence-matrix]" << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << "   | ";
            for (int j = 0; j < vertices.size(); j++) {
                std::cout << (this->edge_matrix[i][j] != nullptr ? "1" : "0") << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << "  [flow-matrix]" << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << "   | ";
            for (int j = 0; j < vertices.size(); j++) {
                Edge* edge = this->edge_matrix[i][j];
                int flow = edge != nullptr ? edge->flow : 0;
                if (flow >= 0) {
                    if (flow <= 9) {
                        std::cout << " " << flow << "  |";
                    } else {
                        std::cout << " " << flow << " |";
                    }
                } else {
                    if (flow >= -9) {
                        std::cout << " " << flow << "  |";
                    } else {
                        std::cout << flow << " |";
                    }
                }
            }
            std::cout << std::endl;
        }
        std::cout << "  [capacity-matrix]" << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << "   | ";
            for (int j = 0; j < vertices.size(); j++) {
                Edge* edge = this->edge_matrix[i][j];
                int capacity = edge != nullptr ? edge->capacity : 0;
                if (capacity >= 0) {
                    if (capacity <= 9) {
                        std::cout << " " << capacity << "  |";
                    } else {
                        std::cout << " " << capacity << " |";
                    }
                } else {
                    if (capacity >= -9) {
                        std::cout << " " << capacity << "  |";
                    } else {
                        std::cout << capacity << " |";
                    }
                }
            }
            std::cout << std::endl;
        }
    }
}

void BaseGraph::setVerbose(bool verbose) {
    this->verbose = verbose;
}

void BaseGraph::preProcess(int s, int t) {
    // the label of the source vertex is set to the number of vertices
    // the label of all the other vertices (different from s) is already set to 0
    this->vertices[s].label = (int) vertices.size();
    // for each edge that goes from source to another vertex, the flow is set equal
    // to the capacity and the excess of the destination vertex is set to the capacity
    for (int i = 0; i < this->vertices.size(); i++) {
        Edge* edge = this->edge_matrix[s][i];
        if (edge != nullptr) {
            edge->flow = edge->capacity;
            this->vertices[i].excess = edge->capacity;
            // the reverse link must be set to the inverse flow
            this->edge_matrix[i][s]->flow = -edge->flow;
        }
    }
}

int BaseGraph::getActiveNode(int s, int t) {
    // this method must be differently implemented in each variation of
    // the graph: this basic implementation will not return any vertex.
    return NO_ACTIVE_NODE_FOUND;
}

/**
 * Try to push the overflowing flow from node u to all the connected nodes.
 * @param u index of the node that have overflowing flow to push.
 * @return if at least one admissible arc exists.
 */
bool BaseGraph::pushFlow(int u) {
    // iterate all the adjacent nodes
    for (int i = 0; i < this->vertices.size(); i++) {
        Edge* edge = this->edge_matrix[u][i];
        if (edge != nullptr) {
            // vertex i is adjacent to vertex u
            if (edge->flow < edge->capacity) {
                // if flow is equal to capacity the push cannot be done
                if (this->vertices[u].label > this->vertices[i].label) {
                    // the push can be done only if the label of the overflowing
                    // vertex if higher than the label of the adjacent one.
                    // the amount of flow that can be sent is equal to the minimum
                    // between the remaining flow on edge and excess flow.
                    int flow = std::min(edge->capacity - edge->flow, this->vertices[u].excess);
                    // this flow can be sent from vertex u to vertex i.
                    // the excessive flow in node u must be reduced.
                    this->vertices[u].excess -= flow;
                    // the incoming flow in node i must be added.
                    this->vertices[i].excess += flow;
                    // the flow on edge u->i must be increased.
                    edge->flow += flow;
                    // also the reverse edge must be updated.
                    this->edge_matrix[i][u]->flow -= flow;
                    // one arc has been found and some flow has been pushed
                    // so we can exit the cycle and return success.
                    if (this->verbose) {
                        std::cout << "=> Flow moved from " << u << " to " << i << " by " << flow << " units" << std::endl;
                    }
                    return true;
                }
            }
        }
    }
    // if no push has been done, return false
    return false;
}

bool BaseGraph::relabel(int u) {
    // we have to look for the minimum label between the adjacent nodes
    int minimum = -1;
    // do the iteration
    for (int i = 0; i < this->vertices.size(); i++) {
        Edge *edge = this->edge_matrix[u][i];
        if (edge != nullptr) {
            // ensure that this edge is not full
            if (edge->flow < edge->capacity) {
                // obtain the label for this adjacent node
                int label = this->vertices[i].label;
                if (minimum == -1 || label < minimum) {
                    // the label of the adjacent node is the minimum
                    minimum = label;
                    this->vertices[u].label = label + 1;
                }
            }
        }
    }
    // return if vertex has been relabeled
    return minimum != -1;
}

void BaseGraph::addEdge(int u, int v, int capacity) {
    this->edge_matrix[u][v] = new Edge(DEFAULT_FLOW, capacity);
    this->edge_matrix[v][u] = new Edge(-DEFAULT_FLOW, 0);
}

int BaseGraph::getMaximumFlow(int s, int t) {
    // the algorithm start pre-processing input data
    preProcess(s, t);
    // print current status after pre-processing
    printCurrentStatus();
    // enter the main cycle
    int cycles = 0;
    int activeNode = getActiveNode(s, t);
    while (activeNode != NO_ACTIVE_NODE_FOUND) {
        // an active node has been found
        if (this->verbose) {
            std::cout << "=> Current active node: " << activeNode << std::endl;
        }
        // try to push some flow in adjacent nodes
        if (!pushFlow(activeNode)) {
            // no admissible arc found
            if (this->verbose) {
                std::cout << "=> No admissible arc found, relabeling node " << activeNode << std::endl;
            }
            // relabel the node
            relabel(activeNode);
        }
        // print current status after pre-processing
        printCurrentStatus();
        // check for another active node
        activeNode = getActiveNode(s, t);
        cycles += 1;
    }
    if (this->verbose) {
        std::cout << "=> Cycles count: " << cycles << std::endl;
    }
    // no more active node found, return the maximum flow
    return this->vertices[t].excess;
}