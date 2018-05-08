#include "redblacktree.h"

RedBlackTree::RedBlackTree() {
    root = nullptr;
}

void RedBlackTree::addNode(Node *node) {
    if (root == nullptr) {
        node->setcolor(BLACK);
        root = node;
        return;
    } else {
        Node* curNode = root;
        while (!curNode->isNill()) {
            if (curNode->data > node->data) {
                curNode->incSize();
                if (curNode->left->isNill()) {
                    delete curNode->left;
                    curNode->left = node;
                    node->parent = curNode;
                    break;
                } else {
                    curNode = curNode->left;
                }
            } else if (curNode->data < node->data) {
                curNode->incSize();
                if (curNode->right->isNill()) {
                    delete curNode->right;
                    curNode->right = node;
                    node->parent = curNode;
                    break;
                } else {
                    curNode = curNode->right;
                }
            } else {
                return;
            }
        }
    }
    Node* tmpNode = node;
    Node* uncle = nullptr;
    while (!tmpNode->isRoot() && tmpNode->getColor() == RED &&
           tmpNode->parent->getColor() == RED) {
        if (tmpNode->isRoot()) {
            tmpNode->setcolor(BLACK);
            break;
        }

        if (tmpNode->parent->isLeftChild()) {
            uncle = tmpNode->parent->parent->right;
        } else {
            uncle = tmpNode->parent->parent->left;
        }

        // CASE A: Uncle is RED
        if (uncle->getColor() == RED) {
            tmpNode->parent->setcolor(BLACK);
            uncle->setcolor(BLACK);
            tmpNode->parent->parent->setcolor(RED);
            tmpNode = tmpNode->parent->parent;
        }
        // CASE B: Uncle is BLACK
        else {
            if (tmpNode->isLeftChild() && tmpNode->parent->isLeftChild()) {
                tmpNode->parent->setcolor(BLACK);
                tmpNode->parent->parent->setcolor(RED);
                tmpNode->parent->parent->rotateRight();
                tmpNode->parent->right->recalcSize();
                tmpNode->parent->recalcSize();
            } else if (tmpNode->isRightChild() && tmpNode->parent->isLeftChild()) {
                tmpNode->setcolor(BLACK);
                tmpNode->parent->parent->setcolor(RED);
                tmpNode->parent->rotateLeft();
                tmpNode->parent->rotateRight();
                tmpNode->left->recalcSize();
                tmpNode->right->recalcSize();
                tmpNode->recalcSize();
            } else if (tmpNode->isRightChild() && tmpNode->parent->isRightChild()) {
                tmpNode->parent->setcolor(BLACK);
                tmpNode->parent->parent->setcolor(RED);
                tmpNode->parent->parent->rotateLeft();
                tmpNode->parent->left->recalcSize();
                tmpNode->parent->recalcSize();
            } else if (tmpNode->isLeftChild() && tmpNode->parent->isRightChild()) {
                tmpNode->setcolor(BLACK);
                tmpNode->parent->parent->setcolor(RED);
                tmpNode->parent->rotateRight();
                tmpNode->parent->rotateLeft();
                tmpNode->left->recalcSize();
                tmpNode->right->recalcSize();
                tmpNode->recalcSize();
            }
        }
    }
    if (tmpNode->isRoot()) {
        tmpNode->setcolor(BLACK);
        root = tmpNode;
    }
    if (tmpNode->parent && tmpNode->parent->isRoot()) {
        root = tmpNode->parent;
    }
}

void RedBlackTree::deleteNode(Node *node) {
    if (node == nullptr) {
        return ;
    }
    Node* curNode = root;
    while (!curNode->isNill()) {
        if (curNode->data > node->data) {
            curNode = curNode->left;
        } else if (curNode->data < node->data) {
            curNode = curNode->right;
        } else {
            Node* deletedNode = inorderSuccessor(curNode);
            curNode->data = deletedNode->data;
            fixDeletion(deletedNode);
            break;
        }
    }
}

void RedBlackTree::addNode(const Data &data) {
    Node* node = new Node(data);

    node->left = new Node(node);
    node->right = new Node(node);

    node->left->nill = node->right->nill = true;
    node->left->size = node->right->size = 0;

    node->left->setcolor(BLACK);
    node->right->setcolor(BLACK);

    addNode(node);
}

void RedBlackTree::deleteNode(const Data &data) {
    Node* node = new Node(data);
    deleteNode(node);
}

void RedBlackTree::printTree(Node* curNode) {
    if (curNode->isNill()) {
        return;
    }
    printTree(curNode->left);
    std::cout << curNode->data.key << ' ';
    printTree(curNode->right);
}

void RedBlackTree::printTree() {

    printTree(root);
}

Node *RedBlackTree::inorderSuccessor(Node *node) {
    if (node->right->isNill()) {
        return node;
    }

    Node* curNode = node->right;
    while(!curNode->left->isNill()) {
        curNode = curNode->left;
    }
    return curNode;
}

void RedBlackTree::fixDeletion(Node *deletedNode) {
    Node* u;
    if (deletedNode->right->isNill()) {
        u = deletedNode->left;
        delete deletedNode->right;
    } else {
        u = deletedNode->right;
        delete deletedNode->left;
    }
    u->parent = deletedNode->parent;
    if (deletedNode->parent && deletedNode->isLeftChild()) {
        deletedNode->parent->left = u;
    } else if (deletedNode->parent && deletedNode->isRightChild()) {
        deletedNode->parent->right = u;
    }
    // Case 2: if either u or v is Red
    if (deletedNode->getColor() == RED || u->getColor() == RED) {
        u->setcolor(BLACK);
        if (u->isRoot()) {
            root = u;
        }
    }
    // Case 3: If Both u and v are Black
    else {
        u->setcolor(DOUBLE_BLACK);
        while (u->getColor() == DOUBLE_BLACK && !u->isRoot()) {
            Node* s = u->parent->right;
            if (u->isRightChild())
                s = u->parent->left;
            // Case A: If sibling s is black and at least one of sibling’s children is red
            if (s->getColor() == BLACK &&
                    (s->left && s->left->getColor() == RED || s->right && s->right->getColor() == RED) ) {
                Node* r;
                if (s->left->getColor() == RED) {
                    r = s->left;
                } else if (s->right->getColor() == RED) {
                    r = s->right;
                }
                r->setcolor(BLACK);
                u->setcolor(BLACK);
                if (s->isLeftChild() && r->isLeftChild()) {
                    s->parent->rotateRight();
                } else if (s->isLeftChild() && r->isRightChild()) {
                    s->rotateLeft();
                    r->parent->rotateRight();
                } else if (s->isRightChild() && r->isRightChild()) {
                    s->parent->rotateLeft();
                } else if (s->isRightChild() && r->isLeftChild()) {
                    s->rotateRight();
                    r->parent->rotateLeft();
                }
                if (u->parent->parent->isRoot()) {
                    root = u->parent->parent;
                }
            }
            // Case B: If sibling is black and its both children are black
            else if (s->getColor() == BLACK &&
                     !(s->left && s->left->getColor() == RED || s->right && s->right->getColor() == RED)) {
                s->setcolor(RED);
                u->setcolor(BLACK);
                if (u->parent->getColor() == BLACK) {
                    u->parent->setcolor(DOUBLE_BLACK);
                } else {
                    u->parent->setcolor(BLACK);
                }
                u = u->parent;
            }
            // Case C: If sibling is red
            else if (s->getColor() == RED) {
                s->setcolor(BLACK);
                u->parent->setcolor(RED);
                if (s->isLeftChild()) {
                    s->parent->rotateRight();
                } else if (s->isRightChild()) {
                    s->parent->rotateLeft();
                }
                if (s->isRoot()) {
                    root = s;
                }
            }

        }
        if (u->isRoot()) {
            u->setcolor(BLACK);
            root = u;
        }
    }
    delete deletedNode;
}

Data& Data::operator=(const Data &curNode) {
    this->key = curNode.key;
    return *this;
}

bool Data::operator <(const Data &data) {
    return this->key < data.key;
}

bool Data::operator <=(const Data &data) {
    return this->key <= data.key;
}


bool Data::operator >(const Data &data) {
    return this->key > data.key;
}

Node::Node() {
    left = right = parent = nullptr;
    color = RED;
    nill = false;
    size = 1;
}

Node::Node(Node *nparent) {
    left = right = nullptr;
    this->parent = nparent;
    color = RED;
    nill = false;
    size = 1;
}

Node::Node(const Data &data, Node* nparent) {
    left = right = nullptr;
    parent = nparent;

    this->data = data;
    color = RED;
    nill = false;
    size = 1;
}

void Node::setcolor(const Color & ncolor) {
    this->color = ncolor;
}

Color Node::getColor() {
    return color;
}

bool Node::isNill() {
    return nill;
}

bool Node::isRoot() {
    return parent == nullptr;
}

bool Node::isLeftChild() {
    return parent->left == this;
}

bool Node::isRightChild() {
    return parent->right == this;
}

void Node::swapData(Node & curNode) {
    Node tmpNode = this->data;
    this->data = curNode.data;
    curNode.data = tmpNode.data;
}

void Node::rotateLeft() {
    if (right->isNill()) {
        return;
    } else {
        Node* tmpRight = right;

        // connect right-left with this
        right->left->parent = this;
        right = right->left;

        // connect parent with right
        tmpRight->parent = parent;
        if (parent && isLeftChild() ) {
            parent->left = tmpRight;
        } else if (parent && isRightChild() ) {
            parent->right = tmpRight;
        }

        // reconnect this with right
        tmpRight->left = this;
        parent = tmpRight;
    }
}

void Node::rotateRight() {
    if (left->isNill()) {
        return;
    } else {
        Node* tmpLeft = left;

        // connect right-left with this
        left->right->parent = this;
        left = left->right;

        // connect parent with right
        tmpLeft->parent = parent;
        if (parent && isRightChild() ) {
            parent->right = tmpLeft;
        } else if (parent && isLeftChild() ) {
            parent->left = tmpLeft;
        }

        // reconnect this with right
        tmpLeft->right = this;
        parent = tmpLeft;
    }
}

void Node::incSize()
{
    size++;
}

void Node::recalcSize() {
    this->size = (left ? left->size : 0) +
            (right ? right->size + 1 : 0);
}

