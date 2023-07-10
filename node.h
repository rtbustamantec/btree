#ifndef NODE_H
#define NODE_H

template <typename TK>
struct Node {
    // array de keys
    TK* keys;
    // array de punteros a hijos
    Node<TK>** children;
    // cantidad de keys
    int count;
    // indicador de nodo hoja
    bool leaf;

    Node() : keys(nullptr), children(nullptr), count(0), leaf(true) {}

    Node(int M) : count(0), leaf(true) {
        keys = new TK[M - 1];
        children = new Node<TK>*[M];
    }

    ~Node() {
        clear();
        delete[] keys;
        delete[] children;
    }

    void clear() {
        if (!leaf) {
            for (int i = 0; i <= count; i++) {
                children[i]->clear();
                delete children[i];
            }
        }
        count = 0;
        leaf = true;
    }

    void splitChild(int index) {
        Node<TK>* child = children[index];
        Node<TK>* newNode = new Node<TK>(M);
        newNode->leaf = child->leaf;

        for (int i = 0; i < M - 1; i++) {
            newNode->keys[i] = child->keys[i + M];
        }

        if (!child->leaf) {
            for (int i = 0; i < M; i++) {
                newNode->children[i] = child->children[i + M];
            }
        }

        child->count = M - 1;

        for (int i = count; i > index; i--) {
            children[i + 1] = children[i];
        }

        children[index + 1] = newNode;

        for (int i = count - 1; i >= index; i--) {
            keys[i + 1] = keys[i];
        }

        keys[index] = child->keys[M - 1];
        count++;
    }

    void insertNonFull(TK key) {
        int i = count - 1;

        if (leaf) {
            while (i >= 0 && key < keys[i]) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = key;
            count++;
        } else {
            while (i >= 0 && key < keys[i]) {
                i--;
            }

            if (children[i + 1]->count == (2 * M) - 1) {
                splitChild(i + 1);
                if (key > keys[i + 1]) {
                    i++;
                }
            }

            children[i + 1]->insertNonFull(key);
        }
    }

    void insert(TK key) {
        int i = count - 1;

        if (leaf) {
            while (i >= 0 && key < keys[i]) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = key;
            count++;
        } else {
            while (i >= 0 && key < keys[i]) {
                i--;
            }

            if (children[i + 1]->count == (2 * M) - 1) {
                splitChild(i + 1);
                if (key > keys[i + 1]) {
                    i++;
                }
            }

            children[i + 1]->insert(key);
        }
    }

    void inorderTraversal(std::stringstream& ss, const std::string& sep) {
        int i;
        for (i = 0; i < count - 1; i++) {
            if (!leaf) {
                children[i]->inorderTraversal(ss, sep);
            }
            ss << keys[i] << sep;
        }

        if (!leaf) {
            children[i]->inorderTraversal(ss, sep);
        }

        ss << keys[i];
    }

    void remove(TK key) {
        int i = 0;
        while (i < count && key > keys[i]) {
            i++;
        }

        if (i < count && key == keys[i]) {
            if (leaf) {
                for (int j = i; j < count - 1; j++) {
                    keys[j] = keys[j + 1];
                }
                count--;
            } else {
                if (children[i]->count >= M) {
                    TK predecessor = getPredecessor(i);
                    keys[i] = predecessor;
                    children[i]->remove(predecessor);
                } else if (children[i + 1]->count >= M) {
                    TK successor = getSuccessor(i);
                    keys[i] = successor;
                    children[i + 1]->remove(successor);
                } else {
                    mergeChildren(i);
                    children[i]->remove(key);
                }
            }
        } else {
            if (leaf) {
                return;
            }

            if (children[i]->count < M) {
                fillChild(i);
            }

            if (i == count && children[i]->count < M) {
                fillChild(i - 1);
            }

            children[i]->remove(key);
        }
    }

private:
    int M;  // grado u orden del arbol

    TK getPredecessor(int index) {
        Node<TK>* current = children[index];
        while (!current->leaf) {
            current = current->children[current->count];
        }
        return current->keys[current->count - 1];
    }

    TK getSuccessor(int index) {
        Node<TK>* current = children[index + 1];
        while (!current->leaf) {
            current = current->children[0];
        }
        return current->keys[0];
    }

    void mergeChildren(int index) {
        Node<TK>* child = children[index];
        Node<TK>* sibling = children[index + 1];

        child->keys[M - 1] = keys[index];

        for (int i = 0; i < sibling->count; i++) {
            child->keys[i + M] = sibling->keys[i];
        }

        if (!child->leaf) {
            for (int i = 0; i <= sibling->count; i++) {
                child->children[i + M] = sibling->children[i];
            }
        }

        for (int i = index + 1; i < count; i++) {
            keys[i - 1] = keys[i];
        }

        for (int i = index + 2; i <= count; i++) {
            children[i - 1] = children[i];
        }

        child->count += sibling->count + 1;
        count--;

        delete sibling;
    }

    void fillChild(int index) {
        if (index != 0 && children[index - 1]->count >= M) {
            borrowFromPrev(index);
        } else if (index != count && children[index + 1]->count >= M) {
            borrowFromNext(index);
        } else {
            if (index != count) {
                mergeChildren(index);
            } else {
                mergeChildren(index - 1);
            }
        }
    }

    void borrowFromPrev(int index) {
        Node<TK>* child = children[index];
        Node<TK>* sibling = children[index - 1];

        for (int i = child->count - 1; i >= 0; i--) {
            child->keys[i + 1] = child->keys[i];
        }

        if (!child->leaf) {
            for (int i = child->count; i >= 0; i--) {
                child->children[i + 1] = child->children[i];
            }
        }

        child->keys[0] = keys[index - 1];

        if (!child->leaf) {
            child->children[0] = sibling->children[sibling->count];
        }

        keys[index - 1] = sibling->keys[sibling->count - 1];

        child->count++;
        sibling->count--;
    }

    void borrowFromNext(int index) {
        Node<TK>* child = children[index];
        Node<TK>* sibling = children[index + 1];

        child->keys[child->count] = keys[index];

        if (!child->leaf) {
            child->children[child->count + 1] = sibling->children[0];
        }

        keys[index] = sibling->keys[0];

        for (int i = 1; i < sibling->count; i++) {
            sibling->keys[i - 1] = sibling->keys[i];
        }

        if (!sibling->leaf) {
            for (int i = 1; i <= sibling->count; i++) {
                sibling->children[i - 1] = sibling->children[i];
            }
        }

        child->count++;
        sibling->count--;
    }
};

#endif