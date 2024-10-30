#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
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
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
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

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept { /* TODO */ }

    public:
        basic_iterator() { /* TODO */ };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { /* TODO */ }
        pointer operator->() const { /* TODO */ }
        basic_iterator &operator++() { /* TODO */ }
        basic_iterator operator++(int) { /* TODO */ }
        bool operator==(const basic_iterator &other) const noexcept { /* TODO */ }
        bool operator!=(const basic_iterator &other) const noexcept { /* TODO */ }
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

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept { /* TODO */ }

        public:
            local_iterator() { /* TODO */ }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { /* TODO */ }
            pointer operator->() const { /* TODO */ }
            local_iterator & operator++() { /* TODO */ }
            local_iterator operator++(int) { /* TODO */ }

            bool operator==(const local_iterator &other) const noexcept { /* TODO */ }
            bool operator!=(const local_iterator &other) const noexcept { /* TODO */ }
    };

private:

    size_type _bucket(size_t code) const {
        return _range_hash(code, _bucket_count);
    }

    size_type _bucket(const Key & key) const {
        return _bucket(_hash(key));
    }

    size_type _bucket(const value_type & val) const {
        return _bucket(val.first);
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) {
        if (_buckets[bucket] == nullptr) {
            static HashNode* null_ptr = nullptr;
            return null_ptr;
        }

        HashNode** current = &_buckets[bucket];
        while (*current != nullptr) {
            if (_equal((*current)->val.first, key)) {
                return *current;
            }
            current = &((*current)->next);
        }

        static HashNode* null_ptr = nullptr;
        return null_ptr;
    }

    HashNode*& _find(const Key & key) {
        size_type code = _hash(key);
        size_type bucket = _bucket(code);
        return _find(code, bucket, key);
    }

    HashNode* _insert_into_bucket(size_type bucket, value_type && value) {
        HashNode* new_node = new HashNode(std::move(value));
        
        // Insert at the beginning of the bucket's linked list
        new_node->next = _buckets[bucket];
        _buckets[bucket] = new_node;

        // Insert into the global linked list
        if (_head == nullptr) {
            _head = new_node;
        } else {
            // Traverse to the end of the global list and append
            HashNode* current = _head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new_node;
        }
        
        _size++;
        return new_node;
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) {
        dst._bucket_count = src._bucket_count;
        dst._buckets = src._buckets;
        dst._head = src._head;
        dst._size = src._size;
        dst._hash = src._hash;
        dst._equal = src._equal;
        
        src._bucket_count = 0;
        src._buckets = nullptr;
        src._head = nullptr;
        src._size = 0;
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash{}, 
                         const key_equal & equal = key_equal{})
        : _bucket_count(next_greater_prime(bucket_count))
        , _buckets(new HashNode*[_bucket_count]())
        , _head(nullptr)
        , _size(0)
        , _hash(hash)
        , _equal(equal) {
        std::fill(_buckets, _buckets + _bucket_count, nullptr);
    }

    ~UnorderedMap() {
        clear();
        delete[] _buckets;
    }

    UnorderedMap(const UnorderedMap & other)
        : _bucket_count(other._bucket_count)
        , _buckets(new HashNode*[other._bucket_count]())
        , _head(nullptr)
        , _size(0)
        , _hash(other._hash)
        , _equal(other._equal) {
        std::fill(_buckets, _buckets + _bucket_count, nullptr);
        
        for (HashNode* curr = other._head; curr != nullptr; curr = curr->next) {
            insert(curr->val);
        }
    }

    UnorderedMap(UnorderedMap && other)
        : _bucket_count(0)
        , _buckets(nullptr)
        , _head(nullptr)
        , _size(0) {
        _move_content(other, *this);
    }

    UnorderedMap & operator=(const UnorderedMap & other) {
        if (this != &other) {
            clear();
            delete[] _buckets;
            
            _bucket_count = other._bucket_count;
            _buckets = new HashNode*[_bucket_count]();
            std::fill(_buckets, _buckets + _bucket_count, nullptr);
            _head = nullptr;
            _size = 0;
            _hash = other._hash;
            _equal = other._equal;
            
            // Copy all nodes
            for (HashNode* curr = other._head; curr != nullptr; curr = curr->next) {
                insert(curr->val);
            }
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) {
        if (this != &other) {
            clear();
            delete[] _buckets;
            _move_content(other, *this);
        }
        return *this;
    }

    void clear() noexcept {
        HashNode* current = _head;
        while (current != nullptr) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
        
        std::fill(_buckets, _buckets + _bucket_count, nullptr);
        _head = nullptr;
        _size = 0;
    }

    size_type size() const noexcept {
        return _size;
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_type bucket_count() const noexcept {
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
    }

    const_iterator cend() const {
        return const_iterator(this, nullptr);
    }

     local_iterator begin(size_type n) {
        return local_iterator(_buckets[n]);
    }

    local_iterator end(size_type n) {
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) {
        size_type count = 0;
        HashNode* current = _buckets[n];
        while (current != nullptr && _bucket(current->val) == n) {
            count++;
            current = current->next;
        }
        return count;
    }

    float load_factor() const {
        return static_cast<float>(_size) / _bucket_count;
    }

    size_type bucket(const Key & key) const {
        return _bucket(key);
    }

    std::pair<iterator, bool> insert(value_type && value) {
        // Check if key already exists
        size_type code = _hash(value.first);
        size_type bucket_idx = _bucket(code);
        HashNode*& existing = _find(code, bucket_idx, value.first);
        
        if (existing != nullptr) {
            return {iterator(this, existing), false};
        }
        
        // Insert new node
        HashNode* new_node = _insert_into_bucket(bucket_idx, std::move(value));
        return {iterator(this, new_node), true};
    }

    std::pair<iterator, bool> insert(const value_type & value) {
        // Check if key already exists
        size_type code = _hash(value.first);
        size_type bucket_idx = _bucket(code);
        HashNode*& existing = _find(code, bucket_idx, value.first);
        
        if (existing != nullptr) {
            return {iterator(this, existing), false};
        }
        
        // Insert new node
        HashNode* new_node = _insert_into_bucket(bucket_idx, value_type(value));
        return {iterator(this, new_node), true};
    }

    iterator find(const Key & key) {
        HashNode*& node = _find(key);
        return iterator(this, node);
    }

    T& operator[](const Key & key) {
        HashNode*& existing = _find(key);
        if (existing != nullptr) {
            return existing->val.second;
        }
        
        auto result = insert(value_type(key, T()));
        return result.first->second;
    }

    iterator erase(iterator pos) {
        if (pos._ptr == nullptr) return pos;
        
        HashNode* current = pos._ptr;
        HashNode* next = current->next;
        size_type bucket_idx = _bucket(current->val);
        
        // Update bucket head if needed
        if (_buckets[bucket_idx] == current) {
            _buckets[bucket_idx] = next;
        }
        
        // Update global head if needed
        if (_head == current) {
            _head = next;
        }
        
        HashNode* prev = nullptr;
        HashNode* temp = _buckets[bucket_idx];
        while (temp != nullptr && temp != current) {
            prev = temp;
            temp = temp->next;
        }
        if (prev != nullptr) {
            prev->next = next;
        }
        
        delete current;
        _size--;
        
        return iterator(this, next);
    }

    size_type erase(const Key & key) {
        HashNode*& node = _find(key);
        if (node == nullptr) {
            return 0;
        }
        
        erase(iterator(this, node));
        return 1;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
