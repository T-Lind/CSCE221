#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"

/*
    Example implementation of UnorderedMap:

    #include "UnorderedMap.h"
    #include <iostream>

    int main() {
        UnorderedMap<int, std::string> map(5);

        // Insert elements
        map.insert({1, "one"});
        map.insert({2, "two"});
        map.insert({3, "three"});

        // Access elements
        std::cout << "Element with key 2: " << map[2] << std::endl;

        // Erase element
        map.erase(2);

        // Check size
        std::cout << "Size after erasing key 2: " << map.size() << std::endl;

        // Print map
        print_map(map);

        return 0;
    }

    Expected output:
    Element with key 2: two
    Size after erasing key 2: 2
    0:
    1: (1, one)
    2:
    3: (3, three)
    4:

    Big O Notation for operations:

    - Insert:
      - Average case: O(1)
      - Worst case: O(n) (when all elements hash to the same bucket)

    - Erase:
      - Average case: O(1)
      - Worst case: O(n) (when all elements hash to the same bucket)

    - Find:
      - Average case: O(1)
      - Worst case: O(n) (when all elements hash to the same bucket)

    - Access (operator[]):
      - Average case: O(1)
      - Worst case: O(n) (when all elements hash to the same bucket)

    - Size: O(1)

    - Empty: O(1)

    - Clear: O(n)

    - Bucket count: O(1)

    - Begin/End: O(1)

    - Load factor: O(1)

    - Bucket size: O(k) (where k is the number of elements in the bucket)

    - Bucket: O(1)

    Example usage of iterators:

    #include "UnorderedMap.h"
    #include <iostream>

    int main() {
        UnorderedMap<int, std::string> map(5);

        // Insert elements
        map.insert({1, "one"});
        map.insert({2, "two"});
        map.insert({3, "three"});

        // Iterate over elements
        for (auto it = map.begin(); it != map.end(); ++it) {
            std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        }

        return 0;
    }

    Expected output:
    Key: 1, Value: one
    Key: 3, Value: three
*/

template<typename Key, typename T, typename Hash = std::hash <Key>, typename Pred = std::equal_to <Key>>
class UnorderedMap {
public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}

        HashNode(const value_type &val, HashNode *next = nullptr) : next{next}, val{val} {}

        HashNode(value_type &&val, HashNode *next = nullptr) : next{next}, val{std::move(val)} {}
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode *_head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

public:

    template<typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;

        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap *_map;
        HashNode *_ptr;

        explicit basic_iterator(UnorderedMap const *map, HashNode *ptr)

        noexcept {
            _map = map;
            _ptr = ptr;
        }

    public:
        basic_iterator() {
            _map = nullptr;
            _ptr = nullptr;
        };

        basic_iterator(const basic_iterator &) = default;

        basic_iterator(basic_iterator &&) = default;

        ~basic_iterator() = default;

        basic_iterator &operator=(const basic_iterator &) = default;

        basic_iterator &operator=(basic_iterator &&) = default;

        reference operator*() const { return _ptr->val; }

        pointer operator->() const { return &(_ptr->val); }

        basic_iterator &operator++() {
            if (_ptr->next != nullptr) {
                _ptr = _ptr->next;
                return *this;
            }

            size_type currBuc = _range_hash(_map->_hash(_ptr->val.first),
                                            _map->_bucket_count); // Get index of current bucket
            for (size_type currentIndex = currBuc + 1; currentIndex < _map->_bucket_count; currentIndex++) {
                HashNode *bct = _map->_buckets[currentIndex];
                if (bct != nullptr) {
                    _ptr = bct;
                    return *this;
                }
            }
            _ptr = nullptr;
            return *this;
        }

        basic_iterator operator++(int) {
            basic_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const basic_iterator &other) const

        noexcept {
            return this->_ptr == other._ptr;
        }

        bool operator!=(const basic_iterator &other) const

        noexcept {
            return this->_ptr != other._ptr;
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const key_type, mapped_type>;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;

        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        HashNode *_node;

        explicit local_iterator(HashNode *node)

        noexcept {
            _node = node;
        }

    public:
        local_iterator() {
            _node = nullptr;
        }

        local_iterator(const local_iterator &) = default;

        local_iterator(local_iterator &&) = default;

        ~local_iterator() = default;

        local_iterator &operator=(const local_iterator &) = default;

        local_iterator &operator=(local_iterator &&) = default;

        reference operator*() const {
            return _node->val;
        }

        pointer operator->() const {
            return &(_node->val);
        }

        local_iterator &operator++() {
            if (_node->next == nullptr) {
                _node = nullptr;
                return *this;
            }
            _node = _node->next;
            return *this;
        }

        local_iterator operator++(int) {
            local_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const local_iterator &other) const

        noexcept {
            return this->_node == other._node;
        }

        bool operator!=(const local_iterator &other) const

        noexcept {
            return this->_node != other._node;
        }
    };

private:

    size_type _bucket(size_t code) const {
        return _range_hash(code, _bucket_count);
    }

    size_type _bucket(const Key &key) const {
        return _range_hash(_hash(key), _bucket_count);
    }

    size_type _bucket(const value_type &val) const {
        return _range_hash(_hash(val.first), _bucket_count);
    }

    HashNode *&_find(size_type code, size_type bucket, const Key &key) {
        HashNode **currentNode = &(_buckets[bucket]);

        while (*currentNode != nullptr) {
            if (_equal((*currentNode)->val.first, key)) {
                return *currentNode;
            }
            currentNode = &((*currentNode)->next);
        }
        return *currentNode;
    }

    HashNode *&_find(const Key &key) {
        return _find(_hash(key), _bucket(key), key);
    }

    HashNode *_insert_into_bucket(size_type bucket, value_type &&value) {
        HashNode *bucketHead = _buckets[bucket];
        auto *nodeToInsert = new HashNode(std::move(value));

        if (bucketHead == nullptr) {
            nodeToInsert->next = nullptr;
            _buckets[bucket] = nodeToInsert;
            _size++;
        } else {
            nodeToInsert->next = bucketHead;
            _buckets[bucket] = nodeToInsert;
            _size++;
        }

        if (_head == nullptr) {
            _head = nodeToInsert;
        } else if (_bucket(value) <= _bucket(_head->val)) {
            _head = nodeToInsert;
        }
        return nodeToInsert;
    }

    void _move_content(UnorderedMap &src, UnorderedMap &dst) {
        dst._hash = std::move(src._hash);
        dst._equal = std::move(src._equal);
        dst._buckets = src._buckets;
        dst._bucket_count = src._bucket_count;
        dst._head = src._head;
        dst._size = src._size;
        src._head = nullptr;
        src._buckets = new HashNode *[src._bucket_count]();
        src._size = 0;

    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash &hash = Hash{},
                          const key_equal &equal = key_equal{}) {
        _head = nullptr;
        _bucket_count = next_greater_prime(bucket_count);
        _buckets = new HashNode *[_bucket_count]();
        _size = 0;
    }

    ~UnorderedMap() {
        clear();
        delete[] _buckets;
        _bucket_count = 0;
        _buckets = nullptr;
        _size = 0;
    }

    UnorderedMap(const UnorderedMap &other) {
        _bucket_count = other._bucket_count;
        _buckets = new HashNode *[_bucket_count]();
        _head = nullptr;
        _size = 0;
        _hash = other._hash;
        _equal = other._equal;

        for (size_type bucketIndex = 0; bucketIndex < other._bucket_count; bucketIndex++) {
            HashNode *curNode = other._buckets[bucketIndex];
            while (curNode != nullptr) {
                insert(curNode->val);
                curNode = curNode->next;
            }
        }
    }

    UnorderedMap(UnorderedMap &&other) {
        _move_content(other, *this);
    }

    UnorderedMap &operator=(const UnorderedMap &other) {
        if (this != &other) {
            this->~UnorderedMap();
            _bucket_count = other._bucket_count;
            _buckets = new HashNode *[_bucket_count]();
            _head = nullptr;
            _size = 0;
            _hash = other._hash;
            _equal = other._equal;

            for (size_type bucketIndex = 0; bucketIndex < other._bucket_count; bucketIndex++) {
                HashNode *curNode = other._buckets[bucketIndex];
                while (curNode != nullptr) {
                    insert(curNode->val);
                    curNode = curNode->next;
                }
            }
        }
        return *this;
    }

    UnorderedMap &operator=(UnorderedMap &&other) {
        if (this != &other) {
            this->~UnorderedMap();
            _move_content(other, *this);
        }
        return *this;
    }

    void clear()

    noexcept {
        if (_size > 0) {
            for (size_type index = 0; index < _bucket_count; index++) {
                HashNode *curNode = _buckets[index];
                if (curNode != nullptr) {
                    while (curNode != nullptr) {
                        HashNode *delNode = curNode;
                        curNode = curNode->next;
                        delete delNode;
                        _size--;
                    }
                }
            }
        }
    }

    size_type size() const

    noexcept {
        return _size;
    }

    bool empty() const

    noexcept {
        return _size == 0;
    }

    size_type bucket_count() const

    noexcept {
        return _bucket_count;
    }

    iterator begin() {
        return iterator(this, _head);
    }

    iterator end() {
        return iterator(this, nullptr);
    }

    const_iterator cbegin() const {
        return const_iterator(this, _head);
    };

    const_iterator cend() const {
        return const_iterator(this, nullptr);
    };

    local_iterator begin(size_type n) {
        return local_iterator(_buckets[n]);
    }

    local_iterator end(size_type n) {
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) {
        size_type count = 0;
        HashNode *curNode = _buckets[n];

        while (curNode != nullptr) {
            count++;
            curNode = curNode->next;
        }
        return count;
    }

    float load_factor() const {
        return ((float) _size) / _bucket_count;
    }

    size_type bucket(const Key &key) const {
        return _bucket(key);
    }

    std::pair<iterator, bool> insert(value_type &&value) {
        HashNode *duplicateCand = _find(value.first);

        if (duplicateCand == nullptr) {
            return std::make_pair(iterator(this, _insert_into_bucket(_bucket(value), std::move(value))), true);
        }
        return std::make_pair(iterator(this, duplicateCand), false);
    }

    std::pair<iterator, bool> insert(const value_type &value) {
        HashNode *duplicateCand = _find(value.first);

        if (duplicateCand == nullptr) {
            HashNode *bucketHead = _buckets[_bucket(value)];
            auto *nodeToInsert = new HashNode(value);

            if (bucketHead == nullptr) {
                nodeToInsert->next = nullptr;
                _buckets[_bucket(value)] = nodeToInsert;
                _size++;
            } else {
                nodeToInsert->next = bucketHead;
                _buckets[_bucket(value)] = nodeToInsert;
                _size++;
            }

            if (_head == nullptr) {
                _head = nodeToInsert;
            } else if (_bucket(value) <= _bucket(_head->val)) {
                _head = nodeToInsert;
            }
            return std::make_pair(iterator(this, nodeToInsert), true);
        }

        return std::make_pair(iterator(this, duplicateCand), false);

    }

    iterator find(const Key &key) { return iterator(this, _find(key)); }

    T &operator[](const Key &key) {
        HashNode *foundNode = _find(key);
        if (foundNode == nullptr) {
            auto *insertNde = new HashNode(std::make_pair(key, T{}), nullptr);
            size_type idx = _bucket(key);
            insertNde->next = _buckets[idx];
            _buckets[idx] = insertNde;
            _size++;

            if (_head == nullptr) {
                _head = insertNde;
            } else if (_bucket(insertNde->val) <= _bucket(_head->val)) {
                _head = insertNde;
            }
            return insertNde->val.second;
        }
        return foundNode->val.second;
    }

    iterator erase(iterator pos) {
        if (pos == end()) {
            return end();
        }

        HashNode *eraseNode = pos._ptr;
        size_type idx = _bucket(pos._ptr->val);
        HashNode *curNode = _buckets[idx];

        if (eraseNode == curNode) {
            auto it = iterator(this, eraseNode);
            it++;
            _buckets[idx] = eraseNode->next;
            if (eraseNode == _head) {
                _head = it._ptr;
            }
            delete eraseNode;
            _size--;
            return it;
        }

        while (curNode != nullptr) {
            if (curNode->next == eraseNode) {
                auto it = iterator(this, eraseNode);
                it++;
                curNode->next = eraseNode->next;
                delete eraseNode;
                _size--;
                return it;
            }
            curNode = curNode->next;
        }

        return end();

    }

    size_type erase(const Key &key) {
        size_type bucketIndex = _bucket(key);
        HashNode *currentNode = _buckets[bucketIndex];

        if (_equal(key, currentNode->val.first)) {
            auto it = iterator(this, currentNode);
            it++;
            _buckets[bucketIndex] = currentNode->next;
            if (_head == currentNode) {
                _head = it._ptr;
            }
            delete currentNode;
            _size--;
            return 1;
        }

        while (currentNode->next != nullptr) {
            if (_equal(key, currentNode->next->val.first)) {
                HashNode *eraseNode = currentNode->next;
                currentNode->next = eraseNode->next;
                delete eraseNode;
                _size--;
                return 1;
            }
            currentNode = currentNode->next;
        }
        return 0;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> &map, std::ostream &os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> &map, std::ostream &os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for (size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const *node = map._buckets[bucket];

        while (node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
