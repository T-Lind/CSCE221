#include "hash_functions.h"

/*
Example usage of hash functions:

#include "hash_functions.h"
#include <iostream>

int main() {
    polynomial_rolling_hash poly_hash;
    fnv1a_hash fnv_hash;

    std::string str = "example";

    // Compute polynomial rolling hash
    size_t poly_hash_value = poly_hash(str);
    std::cout << "Polynomial Rolling Hash of \"" << str << "\": " << poly_hash_value << std::endl;

    // Compute FNV-1a hash
    size_t fnv_hash_value = fnv_hash(str);
    std::cout << "FNV-1a Hash of \"" << str << "\": " << fnv_hash_value << std::endl;

    return 0;
}

Expected output:
Polynomial Rolling Hash of "example": <some_hash_value>
FNV-1a Hash of "example": <some_hash_value>

Big O Notation for hash functions:

- Polynomial Rolling Hash:
  - Time complexity: O(n) (where n is the length of the string)
  - Space complexity: O(1)

- FNV-1a Hash:
  - Time complexity: O(n) (where n is the length of the string)
  - Space complexity: O(1)
*/

#include "hash_functions.h"

size_t polynomial_rolling_hash::operator()(std::string const &str) const {
    size_t hash = 0;
    size_t p = 1;
    for (char c: str) {
        hash += c * p;
        p = (p * 19) % 3298534883309ul;
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

size_t polynomial_rolling_hash::operator()(std::string const &str) const {
    size_t hash = 0;
    size_t p = 1;
    for (char c: str) {
        hash += c * p;
        p = (p * 19) % 3298534883309ul;
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