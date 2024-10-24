#include <iostream>
#include <list>
#include <unordered_set>
#include <vector>

std::list<int> intersection(const std::list<int> &l1, const std::list<int> &l2) {
    std::unordered_set<int> l1_as_a_set(l1.begin(), l1.end());

    std::list<int> results;

    for (auto value: l2) {
        if (l1_as_a_set.find(value) != l1_as_a_set.end()) {
            results.push_front(value);
            l1_as_a_set.erase(value);
        }
    }

    return results;
}

template<typename T>
struct Node {
    Node *next;
    T obj;

    Node(T obj, Node *next = nullptr)
            : obj(obj), next(next) {}
};

template<typename T>
int count_nodes(Node<T> *head) {
    int cnt = 0;
    Node<T> *traverse = head;
    while (traverse != nullptr) {
        cnt++;
        traverse = traverse->next;
    }
    return cnt;
}

int find_max_value(std::vector<int> &vec, int index = 0) {
    if (index == vec.size() - 1) {
        return vec[index];
    }

    int max = find_max_value(vec, index + 1);
    return (vec[index] > max) ? vec[index] : max;
}

template<typename T>
struct NodeBT {
    NodeBT<T> *left;
    NodeBT<T> *right;
    T obj;

    NodeBT(T o, NodeBT<T> *l = nullptr, NodeBT<T> *r = nullptr)
            : obj(o), left(l), right(r) {}
};

template<typename T>
int count_filled_nodes(const NodeBT<T> *node) {
    if (node->left == nullptr && node->right == nullptr) {
        return 0;
    }
    if (node->left != nullptr && node->right != nullptr) {
        return 1 + count_filled_nodes(node->left) + count_filled_nodes(node->right);
    }
    if (node->left != nullptr) {
        return count_filled_nodes(node->left);
    }
    if (node->right != nullptr) {
        return count_filled_nodes(node->right);
    }
    // all cases should be covered, so raise exception here
    throw std::logic_error("Error in count_filled_nodes");
}

int main() {
    const std::list<int> l1 = {1, 4, 87, -3, 4, 5, 6, 19};
    const std::list<int> l2 = {885, 3, 4, -4, 6, 0};

    std::list<int> result1 = intersection(l1, l2);

    const std::list<int> l3 = {14, 15, 17, 18};
    const std::list<int> l4 = {0, 1, 2, 3};

    std::list<int> result2 = intersection(l3, l4);

    std::cout << "Result 1:" << std::endl;
    for (int x: result1) {
        std::cout << x << " ";  // should be 4, 6
    }

    std::cout << "Result 2:" << std::endl;
    for (int x: result2) {
        std::cout << x << " ";  // should be nothing, no intersection
    }

    Node<int> c(3, nullptr);
    Node<int> b(2, &c);
    Node<int> a(1, &b);

    std::cout << "Length of 3-item linked list: " << count_nodes(&a) << std::endl;
    std::cout << "Length of 1 node: " << count_nodes(&c) << std::endl;

    std::vector<int> ex1 = {1, 2, 3, 4, 5, 6, 7, 8, 7, 6};
    std::vector<int> ex2 = {13, 5, 6, 7, 8, 7, 6};
    std::cout << "Max of ex1: " << find_max_value(ex1) << std::endl;
    std::cout << "Max of ex2: " << find_max_value(ex2) << std::endl;

    NodeBT<int> leaf1(1);
    NodeBT<int> leaf2(2);
    NodeBT<int> leaf3(3);
    NodeBT<int> leaf4(4);
    NodeBT<int> st11(11, &leaf1, &leaf2);
    NodeBT<int> st12(12, &leaf3, &leaf4);

    NodeBT<int> st21(21, &st11);
    NodeBT<int> head(22, &st12, &st21);

    std::cout << "Number of filled nodes (should be 3): " << count_filled_nodes(&head) << std::endl;
    std::cout << "Number of filled nodes (should be 1): " << count_filled_nodes(&st11) << std::endl;


}