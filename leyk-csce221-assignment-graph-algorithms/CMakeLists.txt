project(leyk-csce221-assignment-graph-algorithms)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(tests/include)
include_directories(tests/rtest/include)

add_executable(leyk-csce221-assignment-graph-algorithms
        src/dijkstras-helpers.h
        src/graph-algorithms.h
        src/graph-types.h
        src/main.cpp
        src/top-sort-helpers.h
        src/weighted-graph.hpp
)