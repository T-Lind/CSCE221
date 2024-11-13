#pragma once

#include <functional>
#include <utility>
#include <vector>

/**
 * PriorityQueue Class
 *
 * This class implements a priority queue using a binary heap. The priority queue supports the following operations:
 *
 * - `top()`: Returns a const reference to the element at the top of the heap. This operation runs in O(1) time.
 * - `empty()`: Checks if the heap is empty. This operation runs in O(1) time.
 * - `size()`: Returns the number of elements in the heap. This operation runs in O(1) time.
 * - `push(const value_type& value)`: Inserts an element into the heap by copying it. This operation runs in O(log(size())) time.
 * - `push(value_type&& value)`: Inserts an element into the heap by moving it. This operation runs in O(log(size())) time.
 * - `pop()`: Removes the top element from the heap. This operation runs in O(log(size())) time.
 *
 * Implementation Details:
 * - The priority queue is implemented using a binary heap stored in a `std::vector`.
 * - The `upheap` function is used to maintain the heap property when an element is added.
 * - The `downheap` function is used to maintain the heap property when the top element is removed.
 * - The `parent`, `left_child`, and `right_child` helper functions are used to navigate the binary heap.
 *
 * Potential Points of Confusion:
 * - The priority queue is implemented as a max heap by default, meaning the largest element is at the top.
 * - The `Compare` template parameter is used to define the comparison function. By default, it is `std::less<T>`, which makes the heap a max heap.
 * - The `upheap` and `downheap` functions are crucial for maintaining the heap property and ensuring the priority queue operates correctly.
 * - The `push` function has two overloads: one for copying and one for moving elements into the heap.
 * - The `pop` function removes the top element and then re-establishes the heap property by moving the last element to the top and calling `downheap`.
 *
 * Example Usage:
 * \code{.cpp}
 * PriorityQueue<int> pq;
 * pq.push(10);
 * pq.push(20);
 * pq.push(5);
 * std::cout << pq.top() << std::endl; // Outputs: 20
 * pq.pop();
 * std::cout << pq.top() << std::endl; // Outputs: 10
 * pq.push(15);
 * std::cout << pq.top() << std::endl; // Outputs: 15
 * \endcode
 */

template <class T, class Container = std::vector<T>, class Compare = std::less<T>>
class PriorityQueue {
public:
    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;

private:
    Container c;
    value_compare comp;

    size_type parent(size_type index) { return (index - 1) / 2; }
    size_type left_child(size_type index) { return 2 * index + 1; }
    size_type right_child(size_type index) { return 2 * (index + 1); }
    bool is_internal(size_t index) { return left_child(index) < c.size(); }
    bool is_leaf(size_t index) { return left_child(index) >= c.size(); }

    
    /**
     * @brief Recursively (or Iteratively) swap the value at index up the heap until it is in the correct position.
     * 
     * @note This is a max heap, so assume Compare is less, but promote the larger value.
     * 
     * Used by push
     * 
     * O(log(size()))
     * 
     * @param index the current position to swap upwards
     */
    void upheap(size_type index) {
    while (index > 0) {
        size_type p = parent(index);
        if (comp(c[p], c[index])) {  // if parent is less than current (for max heap)
            std::swap(c[p], c[index]);
            index = p;
        } else {
            break;
        }
    }
}
    
    /**
     * @brief Recursively swap the value at index down the heap until it is in the correct position.
     * 
     * @note This is a max heap, so assume Compare is less, so promote the larger value.
     * 
     * Used by pop
     * 
     * O(log(size()))
     * 
     * @param index the current position to swap downwards
     */
void downheap(size_type index) {
    while (is_internal(index)) {
        size_type l = left_child(index);
        size_type r = right_child(index);
        size_type lgst = index;

        if (l < c.size() && comp(c[lgst], c[l])) {
            lgst = l;
        }
        if (r < c.size() && comp(c[lgst], c[r])) {
            lgst = r;
        }

        if (lgst != index) {
            std::swap(c[index], c[lgst]);
            index = lgst;
        } else {
            break;
        }
    }
}


public:
    PriorityQueue() = default;
    PriorityQueue( const PriorityQueue& other ) = default;
    PriorityQueue( PriorityQueue&& other ) = default;
    ~PriorityQueue() = default;
    PriorityQueue& operator=( const PriorityQueue& other ) = default;
    PriorityQueue& operator=( PriorityQueue&& other ) = default;

    /**
     * @brief Return a const reference to the element at the top of the heap.
     * 
     * O(1)
     * 
     * @return const_reference to the element at the top of the heap.
     */
    const_reference top() const {
        return c.front();
    }

    /**
     * @brief Return whether the heap is empty. This is the same as whether the underlying container, c, is empty.
     * 
     * O(1)
     * 
     * @return true c is empty
     * @return false c is not empty
     */
    bool empty() const {
        return c.empty();
    }
    /**
     * @brief Return the number of elements in the heap. This is the same as the number of elements in the underlying container, c.
     * 
     * O(1)
     * 
     * @return size_type of the number of elements in the heap
     */
    size_type size() const {
        return c.size();
    }
	
    /**
     * @brief Inserts element and sorts the underlying container, c.
     * 
     * Uses upheap
     * 
     * O(log(size())) 
     * 
     * @param value inserted by copying into c 
     */
    void push(const value_type& value) {
        c.push_back(value);
            upheap(c.size() - 1);
    }

    /**
     * @brief Inserts element and sorts the underlying container, c.
     * 
     * Uses upheap
     * 
     * O(log(size())) 
     * 
     * @param value inserted by moving into c 
     */
	void push(value_type&& value) {
        c.push_back(std::move(value));
            upheap(c.size() - 1);
    }

    /**
     * @brief Removes the top element.
     * 
     * Uses downheap
     * 
     * O(log(size())) 
     * 
     */
    void pop() {
        if (!empty()) {
            c[0] = std::move(c.back());
            c.pop_back();
            if (!empty()) {
                downheap(0);
            }
        }
    }
};