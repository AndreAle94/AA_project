# cmake_minimum_required(VERSION <specify CMake version here>)
project(AdvancedAlgorithmsProject)

set(CMAKE_CXX_STANDARD 11)

add_executable(AdvancedAlgorithmsProject main.cpp base_graph.h base_graph.cpp generic_graph.h generic_graph.cpp goldberg_algorthm_solver.h goldberg_algorithm_solver.cpp lift_to_front_graph.h lift_to_front_graph.cpp)