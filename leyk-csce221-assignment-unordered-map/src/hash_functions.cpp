#include "hash_functions.h"

size_t polynomial_rolling_hash::operator()(std::string const &str) const {
    const size_t b = 19;
    const size_t m = 3298534883309ul;
    size_t hash = 0;
    size_t p = 1;

    for (char c : str) {
        hash = (hash + (c * p) % m) % m;
        p = (p * b) % m;
    }

    return hash;
}


std::size_t fnv1a_hash::operator()(std::string const &str) const {
    const std::size_t prime = 0x00000100000001B3;
    std::size_t hash = 0xCBF29CE484222325;

    for (char c: str) {
        hash ^= c;
        hash *= prime;
    }

    return hash;
}