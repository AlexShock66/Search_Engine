// owner: Jay Kynerd
// 1.0 AVLNode, insert, exists, find
// 1.5 rule of three
// 2.0 size
// 3.0 save, clear

#ifndef SEARCH_ENGINE_POINTERPALS_AVLTREE_H
#define SEARCH_ENGINE_POINTERPALS_AVLTREE_H

#include <iostream>
#include <cmath>
#include <vector>

template <typename T>
class AVLTree {
private:

    class AVLNode {
    public:
        T data;
        AVLNode *left;
        AVLNode *right;
        int height;
        AVLNode(T payload, AVLNode *l, AVLNode *r, int h = 0) {
            data = payload;
            left = l;
            right = r;
            height = h;
        };
        AVLNode(const AVLNode &copy) {
            this->data = copy.data;
            this->left = copy.left;
            this->right = copy.right;
            this->height = copy.height;
        };
        AVLNode& operator=(const AVLNode &copy) {
            if (this == &copy) {
                return *this;
            }
            this->data = copy.data;
            this->left = copy.left;
            this->right = copy.right;
            this->height = copy.height;
            return *this;
        };
        ~AVLNode() = default;
    };

    AVLNode *root;

    void _insert(T x, AVLNode *&t) {
        if (t == nullptr) {
            t = new AVLNode(x, nullptr, nullptr);
        } else if (x < t->data) {
            _insert(x, t->left);
            // returning from LST of node
            if (abs(height(t->left) - height(t->right)) == 2) {
                if (x < t->left->data) { // case 1
                    rotateWithLeftChild(t);
                } else {				 // case 2
                    doubleWithLeftChild(t);
                }
            }

        } else if (t->data < x) {
            _insert(x, t->right);
            // returning from RST of node
            if (abs(height(t->left) - height(t->right)) == 2) {
                if (t->right->data > x) { // case 3
                    doubleWithRightChild(t);
                } else {				  // case 4
                    rotateWithRightChild(t);
                }
            }

        } else;

        t->height = max(height(t->left), height(t->right)) + 1;
    }

    void rotateWithLeftChild(AVLNode *&k2) {
        AVLNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    void doubleWithLeftChild(AVLNode *&k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void rotateWithRightChild(AVLNode *&k1) {
        AVLNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->right), height(k1->left)) + 1;
        k2->height = max(k1->height, height(k2->right)) + 1;
        k1 = k2;
    }

    void doubleWithRightChild(AVLNode *&k1) {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

    int height(AVLNode *curr) {
        return curr == nullptr ? -1 : curr->height;
    }

    int max(int a, int b) {
        return a > b ? a : b;
    }

    bool _exists(const T &value, AVLNode *curr) {
        if (curr == nullptr) {
            return false;
        }
        else if (curr->data == value) {
            return true;
        } else if (value < curr->data) {
            return _exists(value, curr->left);
        } else if (value > curr->data) {
            return _exists(value, curr->right);
        }
    };

    T& _find(const T &value, AVLNode *curr) {
        if (curr == nullptr);
        else if (value == curr->data) {
            return curr->data;
        } else if (value < curr->data) {
            return _find(value, curr->left);
        } else if (value > curr->data) {
            return _find(value, curr->right);
        }
    };

    void copyTree(AVLNode *curr) {
        this->insert(curr->data);
        if (curr->left != nullptr) {
            copyTree(curr->left);
        }
        if (curr->right != nullptr) {
            copyTree(curr->right);
        }
    };

    int _size(AVLNode *curr) {
        if (curr == nullptr) {
            return 0;
        } else {
            return _size(curr->left) + _size(curr->right) + 1;
        }
    };

    void destroy(AVLNode *deleteMe) {
        if (deleteMe != nullptr) {
            destroy(deleteMe->left);
            destroy(deleteMe->right);
        }
        delete deleteMe;
    };

    void _save(std::vector<T> &library, AVLNode* curr) {
        if (curr == nullptr) {
            return;
        }
        library.push_back(curr->data);
        _save(library, curr->left);
        _save(library, curr->right);
    };

public:
    AVLTree() {
        root = nullptr;
    };
    AVLTree(const AVLTree &copy) {
        copyTree(copy.root);
        this->root = copy.root;
    }
    AVLTree& operator=(const AVLTree &copy) {
        if (this == &copy) {
            return *this;
        }
        this->root = copy.root;
        this->copyTree(copy.root);
        return *this;
    };
    ~AVLTree() {
        destroy(root);
    };
    void insert(T payload) {
        if(root == nullptr) {
            root = new AVLNode(payload, nullptr, nullptr);
        } else {
            _insert(payload, root);
        }
    };
    bool exists(const T &value) {
        return _exists(value, root);
    };
    T& find(const T &value) {
        return _find(value, root);
    };
    int size() {
        return _size(root);
    };
    std::vector<T> save() {
        std::vector<T> library;
        _save(library, root);
        return library;
    };
    void clear() {
        destroy(root);
        root = nullptr;
    }
};

#endif //SEARCH_ENGINE_POINTERPALS_AVLTREE_H