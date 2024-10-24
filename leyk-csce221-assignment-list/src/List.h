#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

/*
List Questions:
What is the time complexity of push_back() in the List implementation?

O(1), as it simply appends the node to the end of the list by adjusting the tail's pointers​(List).
How does the erase() function in List invalidate iterators?

It deletes the node and updates the surrounding nodes' pointers, invalidating the iterator pointing to the deleted node​(List).
How does List ensure constant-time pop_back() operations?

It adjusts the pointers of the second-to-last node and deletes the last node in constant time(List).
What will happen if you attempt to dereference an end iterator in the List class?

Dereferencing the end() iterator is undefined behavior, as it points to the tail sentinel node(List).
What is the role of the basic_iterator class in the List class?

It provides bidirectional iteration through the list using operator overloads like ++, --, *, and ->(List).
What is the time complexity of clear() in the List class?

O(n), as it deletes each node by calling pop_front() repeatedly(List).
Queue Questions:
What is the primary difference between front() and back() in the Queue class?

front() accesses the first element (head of the list), while back() accesses the last element (tail of the list)Queue).
Why does the Queue class rely on the List implementation?

The Queue class uses List as the underlying container for efficient front and back operations (FIFO structure)(Queue).
What is the time complexity of the push() operation in the Queue class?

O(1), as it calls push_back() of the List, which adds an element at the end(Queue).
In what scenarios would using the Queue be preferred over using List directly?

When a FIFO structure is needed, such as in scheduling tasks or buffering data, where elements are processed in the order they were added​(Queue).
 */

template<class T>
class List {
private:
    struct Node {
        Node *next, *prev;
        T data;

        explicit Node(Node *prev = nullptr, Node *next = nullptr)
                : next{next}, prev{prev} {}

        explicit Node(const T &data, Node *prev = nullptr, Node *next = nullptr)
                : next{next}, prev{prev}, data{data} {}

        explicit Node(T &&data, Node *prev = nullptr, Node *next = nullptr)
                : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template<typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = pointer_type;
        using reference = reference_type;

    private:
        friend class List<value_type>;

        using Node = typename List<value_type>::Node;
        Node *node;

        explicit basic_iterator(Node *ptr) noexcept: node{ptr} {}

        explicit basic_iterator(const Node *ptr) noexcept: node{const_cast<Node *>(ptr)} {}

    public:
        basic_iterator() : node{nullptr} {}

        basic_iterator(const basic_iterator &) = default;

        basic_iterator(basic_iterator &&) = default;

        ~basic_iterator() = default;

        basic_iterator &operator=(const basic_iterator &) = default;

        basic_iterator &operator=(basic_iterator &&) = default;

        reference operator*() const {
            return node->data;
        }

        pointer operator->() const {
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator &operator++() {
            node = node->next;
            return *this;
        }

        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator temp = *this;
            node = node->next;
            return temp;
        }

        // Prefix Decrement: --a
        basic_iterator &operator--() {
            node = node->prev;
            return *this;
        }

        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            basic_iterator temp = *this;
            node = node->prev;
            return temp;
        }

        bool operator==(const basic_iterator &other) const noexcept {
            return node == other.node;
        }

        bool operator!=(const basic_iterator &other) const noexcept {
            return node != other.node;
        }
    };

public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = basic_iterator<pointer, reference>;
    using const_iterator = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() : head{}, tail{}, _size{0} {
        head.next = &tail;
        tail.prev = &head;
    }

    List(size_type count, const T &value) : List() {
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    explicit List(size_type count) : List() {
        for (size_type i = 0; i < count; ++i) {
            push_back(T());
        }
    }

    List(const List &other) : List() {
        for (const auto &item: other) {
            push_back(item);
        }
    }


    List(List &&other) noexcept: List() {
        if (this != &other) {
            // simply deal with the head and tail nodes and keep the other nodes in the heap
            if (other.empty()) {
                _size = 0;
            } else {
                head.next = other.head.next;
                tail.prev = other.tail.prev;
                head.next->prev = &head;
                tail.prev->next = &tail;
                _size = other._size;

                other.head.next = &other.tail;
                other.tail.prev = &other.head;
                other._size = 0;
            }
        }
    }


    ~List() {
        clear();
    }

    List &operator=(const List &other) {
        if (this != &other) {
            clear();
            for (const auto &item: other) {
                push_back(item);
            }
        }
        return *this;
    }

    List &operator=(List &&other) noexcept {
        if (this != &other) {
            clear();
            // simply deal with the head and tail nodes and keep the other nodes in the heap
            if (other.empty()) {
                _size = 0;
            } else {
                head.next = other.head.next;
                tail.prev = other.tail.prev;
                head.next->prev = &head;
                tail.prev->next = &tail;
                _size = other._size;

                other.head.next = &other.tail;
                other.tail.prev = &other.head;
                other._size = 0;
            }
        }
        return *this;
    }

    reference front() {
        return head.next->data;
    }

    const_reference front() const {
        return head.next->data;
    }

    reference back() {
        return tail.prev->data;
    }

    const_reference back() const {
        return tail.prev->data;
    }

    iterator begin() noexcept {
        return iterator(head.next);
    }

    const_iterator begin() const noexcept {
        return const_iterator(head.next);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        return iterator(&tail);
    }

    const_iterator end() const noexcept {
        return const_iterator(&tail);
    }

    const_iterator cend() const noexcept {
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        while (!empty()) {
            pop_front();
        }
    }

    void push_back(const T &value) {
        Node *new_node = new Node(value, tail.prev, &tail);
        tail.prev->next = new_node;
        tail.prev = new_node;
        ++_size;
    }

    void push_back(T &&value) {
        Node *new_node = new Node(std::move(value), tail.prev, &tail);
        tail.prev->next = new_node;
        tail.prev = new_node;
        ++_size;
    }

    void push_front(const T &value) {
        Node *new_node = new Node(value, &head, head.next);
        head.next->prev = new_node;
        head.next = new_node;
        ++_size;
    }

    void push_front(T &&value) {
        Node *new_node = new Node(std::move(value), &head, head.next);
        head.next->prev = new_node;
        head.next = new_node;
        ++_size;
    }

    void pop_front() {
        if (!empty()) {
            Node *temp = head.next;
            head.next = head.next->next;
            head.next->prev = &head;
            delete temp;
            --_size;
        }
    }


    void pop_back() {
        if (!empty()) {
            Node *temp = tail.prev;
            tail.prev = tail.prev->prev;
            tail.prev->next = &tail;
            delete temp;
            --_size;
        }
    }

    iterator insert(const_iterator pos, const T &value) {
        Node *current = pos.node;
        Node *new_node = new Node(value, current->prev, current);
        current->prev->next = new_node;
        current->prev = new_node;
        ++_size;
        return iterator(new_node);
    }

    iterator insert(const_iterator pos, T &&value) {
        Node *current = pos.node;
        Node *new_node = new Node(std::move(value), current->prev, current);
        current->prev->next = new_node;
        current->prev = new_node;
        ++_size;
        return iterator(new_node);
    }

    iterator erase(const_iterator pos) {
        Node *current = pos.node;
        iterator next_iter(current->next);
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        --_size;
        return next_iter;
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert(iterator pos, const T &value) {
        return insert((const_iterator &) (pos), value);
    }

    iterator insert(iterator pos, T &&value) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase(iterator pos) {
        return erase((const_iterator &) (pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/

namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
            std::is_same<
                    typename List<typename std::iterator_traits<Iter>::value_type>::iterator,
                    Iter
            >{
            }
            && std::is_same<
                    typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
                    ConstIter
            >{
            }, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator &lhs, const ConstIter &rhs) {
    return (const ConstIter &) (lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter &lhs, const Iterator &rhs) {
    return (const ConstIter &) (rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator &lhs, const ConstIter &rhs) {
    return (const ConstIter &) (lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter &lhs, const Iterator &rhs) {
    return (const ConstIter &) (rhs) != lhs;
}

template<class T>
bool operator==(const List<T> &lhs, const List<T> &rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    auto iter1 = lhs.begin();
    auto iter2 = rhs.begin();
    while (iter1 != lhs.end() && iter2 != rhs.end()) {
        if (*iter1 != *iter2) {
            return false;
        }
        iter1++;
        iter2++;
    }
    return true;
}