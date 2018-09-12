#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <random>
#include "goldberg_algorthm_solver.h"
#include "lift_to_front_graph.h"
#include "generic_graph.h"

#define MODE_GENERIC 0
#define MODE_LIFT_TO_FRONT 1
#define DEBUG_MODE false

struct FileLine {

    int u;
    int v;
    int capacity;

    FileLine(int u, int v, int capacity) {
        this->u = u;
        this->v = v;
        this->capacity = capacity;
    }
};

struct BenchmarkResult {

    long time_generic;
    long memory_generic;
    long time_lift_to_front;
    long memory_lift_to_front;

};

std::vector<FileLine> readGraphFromFile(char* file);

BenchmarkResult execute_benchmark(int vertices);

int getGraphVertexCount(std::vector<FileLine> lines);

int main(int argc, char* argv[]) {
    // basic parameters
    bool test_mode = false;
    bool v = DEBUG_MODE;
    // standard solver parameters
    char* file = nullptr;
    int s = -1;
    int t = -1;
    int m = 0;
    // benchmark parameters
    char* folder = nullptr;
    int min = 0;
    int max = 0;
    // parse those parameters from the cli command line
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            // the next argument should be the file name
            file = argv[i+1];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            // the next argument should be the output folder
            folder = argv[i+1];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            // the next argument should be the source vertex
            s = std::stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            // the next argument should be the destination vertex
            t = std::stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            // the next argument should be the solver algorithm
            m = std::stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-v") == 0) {
            // this is a flag and it means that the solver must
            // be executed in verbose mode.
            v = true;
        } else if (strcmp(argv[i], "--test") == 0 && i + 2 < argc) {
            // this is the test mode, the user must specify the lower and upper bound
            test_mode = true;
            min = std::stoi(argv[i+1]);
            max = std::stoi(argv[i+2]);
        }
    }
    if (test_mode) {
        // validate the parameters
        if (folder == nullptr) {
            std::cerr << "Missing -o /path/to/destination argument. Usage: solver --test 1 100 -o /path/to/output/folder [-v to enable verbose mode]";
            return 1;
        }
        // open both output files
        const std::string folder_name(folder);
        std::ofstream output_time_generic(folder_name + "/output_time_generic.txt");
        std::ofstream output_time_lift_to_front(folder_name + "/output_time_lift_to_front.txt");
        std::ofstream output_time_difference(folder_name + "/output_time_difference.txt");
        std::ofstream output_memory_generic(folder_name + "/output_memory_generic.txt");
        std::ofstream output_memory_lift_to_front(folder_name + "/output_memory_lift_to_front.txt");
        std::ofstream output_memory_difference(folder_name + "/output_memory_difference.txt");
        // execute all the tests
        for (int i = min; i <= max; i++) {
            // execute the benchmark
            BenchmarkResult result = execute_benchmark(i);
            // write the results inside the two files
            output_time_generic << i << ", " << result.time_generic << std::endl;
            output_time_lift_to_front << i << ", " << result.time_lift_to_front << std::endl;
            output_time_difference << i << ", " << result.time_generic - result.time_lift_to_front << std::endl;
            output_memory_generic << i << ", " << result.memory_generic << std::endl;
            output_memory_lift_to_front << i << ", " << result.memory_lift_to_front << std::endl;
            output_memory_difference << i << ", " << result.memory_lift_to_front - result.memory_generic << std::endl;
            // print debug info
            if (v) {
                std::cout << "[1] vertices: " << i << " time[ms]: " << result.time_generic <<  " memory[bytes]: " << result.memory_generic << std::endl;
                std::cout << "[2] vertices: " << i << " time[ms]: " << result.time_lift_to_front << " memory[bytes]: " << result.memory_lift_to_front << std::endl;
            }
        }
        // flush the output files
        output_time_generic.flush();
        output_time_lift_to_front.flush();
        output_time_difference.flush();
        output_memory_generic.flush();
        output_memory_lift_to_front.flush();
        output_memory_difference.flush();
        // close the output files
        output_time_generic.close();
        output_time_lift_to_front.close();
        output_time_difference.close();
        output_memory_generic.close();
        output_memory_lift_to_front.close();
        output_memory_difference.close();
    } else {
        // now validate those parameters
        if (file == nullptr) {
            std::cerr << "Missing -i input.txt argument. Usage: solver -i [source file] -s [source vertex] -t [destination vertex] -m [0: generic solver | 1: lift-to-front solver] [-v to enable verbose mode]";
            return 1;
        }
        if (s == -1 || t == -1) {
            std::cerr << "Missing -s and -t arguments. Usage: solver -i [source file] -s [source vertex] -t [destination vertex] -m [0: generic solver | 1: lift-to-front solver] [-v to enable verbose mode]";
            return 1;
        }
        if (m != MODE_GENERIC && m != MODE_LIFT_TO_FRONT) {
            std::cerr << "Invalid solver mode. Usage: solver -i [source file] -s [source vertex] -t [destination vertex] -m [0: generic solver | 1: lift-to-front solver] [-v to enable verbose mode]";
            return 1;
        }
        // we can now open the file and read it
        std::vector<FileLine> lines = readGraphFromFile(file);
        int vertexCount = getGraphVertexCount(lines);
        SolverType type = m == GENERIC_SOLVER ? SolverType::GENERIC_SOLVER : SolverType::LIFT_TO_FRONT_SOLVER;
        // create an instance of the solver object
        GoldbergProblemSolver solver(vertexCount, type, v);
        // fill the graph using file data
        for (auto &line : lines) {
            solver.addEdge(line.u, line.v, line.capacity);
        }
        // calculate the maximum flow between two nodes
        int flow = solver.getMaximumFlow(s, t);
        std::cout << "The maximum flow is: " << flow << std::endl;
    }
    return 0;
}

std::vector<FileLine> readGraphFromFile(char* file) {
    std::ifstream infile(file);
    std::vector<FileLine> lines;
    int u, v, capacity;
    while (infile >> u >> v >> capacity) {
        lines.emplace_back(u, v, capacity);
    }
    return lines;
}

int getGraphVertexCount(std::vector<FileLine> lines) {
    int maximum = -1;
    for (auto &line : lines) {
        if (line.u > maximum) {
            maximum = line.u;
        }
        if (line.v > maximum) {
            maximum = line.v;
        }
    }
    return maximum + 1;
}

BenchmarkResult execute_benchmark(int vertices) {
    // initialize the random library
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> generator(0, 50);
    // keep track of the number of edges to correctly calculate the memory used
    int edge_count = 0;
    // create the adjacent matrix and initialize both graph with the same data
    GenericGraph genericGraph(vertices);
    LiftToFrontGraph liftToFrontGraph(vertices);
    for(int u = 0; u < vertices; u++) {
        for (int v = u; v < vertices; v++) {
            if(v != u) {
                auto weight = static_cast<int>(generator(rng));
                if (weight > 0) {
                    edge_count += 1;
                    genericGraph.addEdge(u, v, weight);
                    liftToFrontGraph.addEdge(u, v, weight);
                }
            }
        }
    }
    // ########################################################################
    // #                         GENERIC ALGORITHM                            #
    // ########################################################################
    // before starting, take note of the time
    long start_generic = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    // it's time to start the algorithm
    genericGraph.getMaximumFlow(0, vertices - 1);
    // algorithm has finished, take note of the time
    long end_generic = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    // we can now see how much time has been consumed
    long time_generic = end_generic - start_generic;
    // calculate the size of memory used by the algorithm
    long memory_generic =
                    (vertices * sizeof(Vertex)) +
                    sizeof(Edge*) + (vertices * vertices * sizeof(Edge*)) +
                    (2 * edge_count * sizeof(Edge));
    // ########################################################################
    // #                      LIFT TO FRONT ALGORITHM                         #
    // ########################################################################
    // before starting, take note of the time and memory
    auto start_lift_to_front = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    // it's time to start the algorithm
    liftToFrontGraph.getMaximumFlow(0, vertices - 1);
    // algorithm has finished, take note of the time
    auto end_lift_to_front = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    // we can now see how much time has been consumed
    long time_lift_to_front = end_lift_to_front - start_lift_to_front;
    // calculate the size of memory used by the algorithm
    long memory_lift_to_front =
                    (vertices * sizeof(Vertex)) +
                    sizeof(Edge*) + (vertices * vertices * sizeof(Edge*)) +
                    (2 * edge_count * sizeof(Edge) +
                     (vertices * sizeof(int)));
    // return the result to the caller, the memory used is released
    return {time_generic, memory_generic, time_lift_to_front, memory_lift_to_front};
}