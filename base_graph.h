//
// Created by andrea on 09/09/18.
//

#ifndef ADVANCEDALGORITHMSPROJECT_GRAPH_H
#define ADVANCEDALGORITHMSPROJECT_GRAPH_H

#include <vector>

#define DEFAULT_LABEL 0
#define DEFAULT_EXCESS 0
#define DEFAULT_FLOW 0

#define NO_ACTIVE_NODE_FOUND (-1)

struct Vertex {

    int label;
    int excess;

    Vertex(int label, int excess) {
        this->label = label;
        this->excess = excess;
    }

};

struct Edge {

    int flow;
    int capacity;

    Edge(int flow, int capacity) {
        this->flow = flow;
        this->capacity = capacity;
    }

};

class BaseGraph {

protected:

    std::vector<Vertex> vertices;

    Edge*** edge_matrix;

    bool verbose;

    virtual void printCurrentStatus();

    virtual void preProcess(int s, int t);

    virtual int getActiveNode(int s, int t);

    virtual bool pushFlow(int u);

    virtual bool relabel(int u);

public:

    explicit BaseGraph(int vertices_count);

    virtual ~BaseGraph();

    void setVerbose(bool verbose);

    void addEdge(int u, int v, int capacity);

    int getMaximumFlow(int s, int t);
};

#endif //ADVANCEDALGORITHMSPROJECT_GRAPH_H