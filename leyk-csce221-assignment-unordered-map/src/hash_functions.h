#pragma once

#include <string>
#include <cstddef>

struct polynomial_rolling_hash {
    std::size_t operator()(std::string const &str) const;
};

struct fnv1a_hash {
    std::size_t operator()(std::string const &str) const;
};

