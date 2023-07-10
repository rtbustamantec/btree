
#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <sstream>
#include "node.h"

template <typename TK>
class BTree {
private:
    Node<TK>* root;
    int M;  // grado u orden del árbol
    int n;  // total de elementos en el árbol

public:
    BTree(int _M) : root(nullptr), M(_M), n(0) {}

    bool search(TK key);  // indica si se encuentra o no un elemento
    void insert(TK key);  // inserta un elemento
    void remove(TK key);  // elimina un elemento
    int height();         // altura del árbol. Considerar altura 0 para árbol vacío
    std::string toString(const std::string& sep);  // recorrido inorder

    TK minKey();  // mínimo valor de la llave en el árbol
    TK maxKey();  // máximo valor de la llave en el árbol
    void clear(); // eliminar todos los elementos del árbol
    int size();   // retorna el total de elementos insertados
    std::vector<int> searchRange(int low, int high);
    ~BTree();     // liberar memoria
};

template <typename TK>
bool BTree<TK>::search(TK key) {
    Node<TK>* current = root;

    while (current) {
        int i = 0;
        while (i < current->count && key > current->keys[i]) {
            i++;
        }

        if (i < current->count && key == current->keys[i]) {
            return true;
        }

        if (current->leaf) {
            return false;
        }

        current = current->children[i];
    }

    return false;
}

template <typename TK>
void BTree<TK>::insert(TK key) {
    if (!root) {
        root = new Node<TK>(M);
        root->insertNonFull(key);
        n++;
    } else {
        if (root->count == (2 * M) - 1) {
            Node<TK>* newRoot = new Node<TK>(M);
            newRoot->children[0] = root;
            root = newRoot;
            root->splitChild(0);
        }
        root->insertNonFull(key);
        n++;
    }
}

template <typename TK>
void BTree<TK>::remove(TK key) {
    if (!root) {
        return;
    }

    root->remove(key);

    if (root->count == 0) {
        Node<TK>* oldRoot = root;
        if (root->leaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete oldRoot;
    }
    n--;
}

template <typename TK>
int BTree<TK>::height() {
    if (!root) {
        return 0;
    }

    Node<TK>* current = root;
    int height = 1;

    while (!current->leaf) {
        current = current->children[0];
        height++;
    }

    return height;
}

template <typename TK>
std::string BTree<TK>::toString(const std::string& sep) {
    std::stringstream ss;
    if (root) {
        root->inorderTraversal(ss, sep);
    }
    return ss.str();
}

template <typename TK>
TK BTree<TK>::minKey() {
    if (!root) {
        throw std::runtime_error("Empty tree");
    }

    Node<TK>* current = root;

    while (!current->leaf) {
        current = current->children[0];
    }

    return current->keys[0];
}

template <typename TK>
TK BTree<TK>::maxKey() {
    if (!root) {
        throw std::runtime_error("Empty tree");
    }

    Node<TK>* current = root;

    while (!current->leaf) {
        current = current->children[current->count];
    }

    return current->keys[current->count - 1];
}

template <typename TK>
void BTree<TK>::clear() {
    if (root) {
        root->clear();
        delete root;
        root = nullptr;
        n = 0;
    }
}

template <typename TK>
int BTree<TK>::size() {
    return n;
}

template <typename TK>
std::vector<int> BTree<TK>::searchRange(int low, int high) {
    std::vector<TK> result;
    if (root) {
        root->searchRange(low, high, result);
    }
    return result;
}

template <typename TK>
BTree<TK>::~BTree() {
    clear();
}

#endif
