#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <iostream>


enum Color {
    RED, BLACK, DOUBLE_BLACK
};

struct Data {
    int key;
    Data() : key(0){}
    Data(int key) : key(key) {}

    Data& operator =(const Data&);
    bool operator >(const Data&);
    bool operator <(const Data&);
    bool operator <=(const Data&);

    friend std::ostream& operator<<(std::ostream& os, const Data& data) {
        os << data.key;
        return os;
    }
};


struct Node {
    Data data;
    Color color;
    bool nill;
    int size;

    Node* left;
    Node* right;
    Node* parent;

    Node();
    Node(Node* parent = nullptr);
    Node(const Data& data, Node* nparent = nullptr);

    void setcolor(const Color&);
    Color getColor();

    bool isNill();
    bool isRoot();
    bool isLeftChild();
    bool isRightChild();

    void swapData(Node&);

    void rotateLeft();
    void rotateRight();

    void incSize();
    void recalcSize();
};

class RedBlackTree {

    Node* root;

public:
    RedBlackTree();

    void addNode(Node*);
    void deleteNode(Node*);

    void addNode(const Data&);
    void deleteNode(const Data&);

    void printTree(Node* curNode);
    void printTree();

    Node* inorderSuccessor(Node* node);
    void fixDeletion(Node* deletedNode);

};

#endif // REDBLACKTREE_H
