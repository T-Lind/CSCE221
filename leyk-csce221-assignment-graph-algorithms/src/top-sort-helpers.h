#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        indegrees[it->first] = 0;
    }
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        const auto& adj_list = it->second;
        for (auto adj_it = adj_list.begin(); adj_it != adj_list.end(); ++adj_it) {
            indegrees[adj_it->first]++;
        }
    }
}
