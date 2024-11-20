#pragma once

#include <istream>
#include <ostream>
#include <sstream>
#include <limits>
#include <list>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "weighted-graph.hpp"

#include "graph-types.h"

// if the arrow is a box, change to the other line
#define ARROW_SEPARATOR " \u2192 "
// #define ARROW_SEPARATOR " -> "

// contains initializeSingleSource, relax, & updateHeap as well as the DijkstraComaparator
#include "dijkstras-helpers.h"

/**
 * @brief Dijkstra's Algorithm - https://canvas.tamu.edu/courses/136654/files/35930572/preview Slide 16
 *
 * @tparam T type of data stored by a vertex
 * @param graph weighted, directed graph to find single-source shortest-path
 * @param initial_node source node in graph for shortest path
 * @param destination_node destination node in graph for shortest path
 * @return std::list<value_type<T>> list of nodes along shortest path including initial_node and destination_node, empty if no path exists
 */
template<typename T>
std::list<value_type<T>> 
dijkstrasAlgorithm(const WeightedGraph<T> &graph, vertex_type<T> initial_node, vertex_type<T> destination_node) {
    std::unordered_map<value_type<T>, weight_type<T>> distances;
    std::unordered_map<value_type<T>, std::optional<value_type<T>>> predecessors;
    std::unordered_set<value_type<T>> s;
    std::priority_queue<value_type<T>, std::vector<value_type<T>>, DijkstraComparator<T>> q(
            DijkstraComparator<T>{distances});

    initializeSingleSource(graph, initial_node, distances, predecessors);
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        q.push(it->first);
    }
    while (!q.empty()) {
        value_type<T> u = q.top();
        q.pop();
        if (distances[u] == infinity<T>()) {
            break;
        }
        s.insert(u);
        const auto& adj_list = graph.at(u);
        for (const auto& [v, w] : adj_list) {
            if (s.find(v) == s.end()) {
                if (relax<T>(u, v, w, distances, predecessors)) {
                    updateHeap(q, distances);
                }
            }
        }
    }

    std::list<value_type<T>> path;
    if (initial_node == destination_node) {
        path.push_back(initial_node);
        return path;
    }
    auto current = destination_node;
    while (predecessors[current].has_value()) {
        path.push_front(current);
        current = predecessors[current].value();
        if (current == initial_node) {
            path.push_front(initial_node);
            return path;
        }
    }
    path.clear();
    return path;
}


#include "top-sort-helpers.h"

/**
 * @brief Returns a Topological Ordering of the Graph - https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
 *
 * @tparam T type of data stored by a vertex
 * @param graph graph upon which to perform a topological ordering
 * @return std::list<value_type<T>> list of nodes in a topological order, or an empty list if no such ordering exists
 */
template<typename T>
std::list<value_type<T>> topologicalSort(const WeightedGraph<T> &graph) {
    std::unordered_map<value_type<T>, int> indegrees;
    computeIndegrees(graph, indegrees);

    std::queue<value_type<T>> q;
    for (const auto& [vertex, indegree] : indegrees) {
        if (indegree == 0) {
            q.push(vertex);
        }
    }

    std::list<value_type<T>> topological_order;
    while (!q.empty()) {
        value_type<T> u = q.front();
        q.pop();
        topological_order.push_back(u);

        const auto& adj_list = graph.at(u);
        for (const auto& [v, _] : adj_list) {
            if (--indegrees[v] == 0) {
                q.push(v);
            }
        }
    }

    if (topological_order.size() != graph.size()) {
        topological_order.clear();
    }

    return topological_order;
}

template<typename T>
std::ostream &operator<<(std::ostream &o, const WeightedGraph<T> &graph) {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        const vertex_type<T> &vertex = it->first;
        const adjacency_list<T> &list = it->second;
        o << vertex << ": ";
        for (auto iit = list.begin(); iit != list.end(); ++iit) {
            const vertex_type<T> &destination = iit->first;
            const weight_type<T> &weight = iit->second;
            o << destination << "(" << weight << ')';
            auto next_iter = iit;
            if (++next_iter != list.end()) {
                o << ARROW_SEPARATOR;
            }
        }
        auto next_iter = it;
        if (++next_iter != graph.end()) {
            o << "\n";
        }
    }
    return o;
}

template<typename T>
std::istream &readEdge(std::istream &i, value_type<T> &vertex, weight_type<T> &weight) {
    std::string s_vertex, s_weight;
    std::getline(i, s_vertex, '(');
    std::getline(i, s_weight, ')');
    std::istringstream stream(s_vertex + " " + s_weight);
    stream >> vertex >> weight;
    if (stream.fail()) {
        i.setf(stream.flags());
    }
    return i;
}

template<typename T>
std::istream &operator>>(std::istream &i, WeightedGraph<T> &graph) {
    std::string line;
    while (std::getline(i, line)) {
        if (line.empty())
            break;
        std::istringstream line_stream(line);
        value_type<T> vertex;
        std::string s_vertex;
        std::getline(line_stream, s_vertex, ':');
        std::istringstream stream_vertex(s_vertex);
        stream_vertex >> vertex;
        if (stream_vertex.fail()) {
            i.setf(stream_vertex.flags());
            break;
        }
        graph.push_vertex(vertex);
        std::string separator;
        value_type<T> edge_end;
        weight_type<T> edge_weight;
        while (readEdge<T>(line_stream, edge_end, edge_weight)) {
            line_stream >> separator;
            graph.push_edge(vertex, edge_end, edge_weight);
        }
    }

    if (i.eof() and i.fail())
        i.clear(std::ios::eofbit);

    return i;
}
