#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

class MinHeap {
private:
    std::vector<int> data;

    void heapifyUp(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;

            if (data[parent] <= data[index]) {
                break;
            }

            std::swap(data[parent], data[index]);
            index = parent;
        }
    }

    void heapifyDown(std::size_t index) {
        while (true) {
            std::size_t left = 2 * index + 1;
            std::size_t right = 2 * index + 2;
            std::size_t smallest = index;

            if (left < data.size() && data[left] < data[smallest]) {
                smallest = left;
            }

            if (right < data.size() && data[right] < data[smallest]) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            std::swap(data[index], data[smallest]);
            index = smallest;
        }
    }

public:
    void build(const std::vector<int>& values) {
        data = values;

        if (!data.empty()) {
            for (int i = static_cast<int>(data.size() / 2) - 1; i >= 0; i--) {
                heapifyDown(static_cast<std::size_t>(i));
            }
        }
    }

    void insert(int value) {
        data.push_back(value);
        heapifyUp(data.size() - 1);
    }

    int peekMin() const {
        if (data.empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        return data[0];
    }

    int removeMin() {
        if (data.empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        int minimum = data[0];
        data[0] = data.back();
        data.pop_back();

        if (!data.empty()) {
            heapifyDown(0);
        }

        return minimum;
    }

    void printArray() const {
        std::cout << "[";

        for (std::size_t i = 0; i < data.size(); i++) {
            std::cout << data[i];

            if (i + 1 < data.size()) {
                std::cout << ", ";
            }
        }

        std::cout << "]\n";
    }
};

class Treap {
private:
    struct Node {
        int key;
        int priority;
        Node* left;
        Node* right;

        Node(int keyValue, int priorityValue)
            : key(keyValue),
              priority(priorityValue),
              left(nullptr),
              right(nullptr) {}
    };

    Node* root = nullptr;

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    Node* insert(Node* node, int key, int priority) {
        if (node == nullptr) {
            return new Node(key, priority);
        }

        if (key == node->key) {
            return node;
        }

        if (key < node->key) {
            node->left = insert(node->left, key, priority);

            // Max-heap priority rule
            if (node->left->priority > node->priority) {
                node = rotateRight(node);
            }
        } else {
            node->right = insert(node->right, key, priority);

            // Max-heap priority rule
            if (node->right->priority > node->priority) {
                node = rotateLeft(node);
            }
        }

        return node;
    }

    bool search(Node* node, int key) const {
        if (node == nullptr) {
            return false;
        }

        if (key == node->key) {
            return true;
        }

        if (key < node->key) {
            return search(node->left, key);
        }

        return search(node->right, key);
    }

    void printInorder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        printInorder(node->left);
        std::cout << node->key << "(" << node->priority << ") ";
        printInorder(node->right);
    }

    void printPreorder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        std::cout << node->key << "(" << node->priority << ") ";
        printPreorder(node->left);
        printPreorder(node->right);
    }

    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    ~Treap() {
        clear(root);
    }

    void insert(int key, int priority) {
        root = insert(root, key, priority);
    }

    bool contains(int key) const {
        return search(root, key);
    }

    void printInorder() const {
        printInorder(root);
        std::cout << "\n";
    }

    void printPreorder() const {
        printPreorder(root);
        std::cout << "\n";
    }
};

int main() {
    std::cout << "===== PART 1: MIN-HEAP =====\n";

    MinHeap heap;

    std::vector<int> values = {
        42, 17, 8, 99, 23, 4,
        55, 12, 31, 6, 73, 1
    };

    heap.build(values);

    std::cout << "Heap after building:\n";
    heap.printArray();

    std::cout << "Minimum value: "
              << heap.peekMin() << "\n\n";

    std::cout << "Removing three minimum values:\n";

    for (int i = 0; i < 3; i++) {
        std::cout << "Removed: " << heap.removeMin() << "\n";
        std::cout << "Heap now: ";
        heap.printArray();
    }

    std::cout << "\nInserting 2 and 15:\n";
    heap.insert(2);
    heap.insert(15);
    heap.printArray();

    std::cout << "\n===== PART 2: TREAP =====\n";

    Treap treap;

    std::vector<std::pair<int, int>> keyPriorityPairs = {
        {50, 40},
        {30, 20},
        {70, 60},
        {20, 10},
        {40, 50},
        {60, 30},
        {80, 70},
        {10, 15},
        {25, 35},
        {35, 45},
        {65, 55},
        {75, 25}
    };

    for (const auto& pair : keyPriorityPairs) {
        treap.insert(pair.first, pair.second);
    }

    std::cout << "Treap inorder traversal:\n";
    std::cout << "Format: key(priority)\n";
    treap.printInorder();

    std::cout << "\nTreap preorder traversal:\n";
    treap.printPreorder();

    std::cout << "\nSearch results:\n";
    std::cout << "Search for key 60: "
              << (treap.contains(60) ? "Found" : "Not found") << "\n";

    std::cout << "Search for key 100: "
              << (treap.contains(100) ? "Found" : "Not found") << "\n";

    return 0;
}