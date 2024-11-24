#ifndef AVL_TREE_H
#define AVL_TREE_H

#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

// AvlTree class
// This implementation is based on the unbalanced binary search tree and adds hight information 
// to the nodes and a balance function to perform the needed rotations.

template <typename Key, typename Value> // string, int
class AvlTree
{
private:
    struct AvlNode
    {
        Key key;
        map<Value, int> valueAndRelevency; // The Value will be the document's position in the "data" vector
        AvlNode *left;
        AvlNode *right;
        int height;      // AVL tree: keeping track of the height is the differnce to a unbalanced binary search tree

        AvlNode(const Key &theKey, AvlNode *lt, AvlNode *rt, int h, map<Value, int> theValueAndRelevency)
            : key{theKey}, left{lt}, right{rt}, height{h}, valueAndRelevency{theValueAndRelevency} {}
    };

    AvlNode *root;
    size_t size;

public:
    /**
     * @brief Default constructor
     */
    AvlTree() : root{nullptr}
    {
        size = 0;
    }

    /**
     * @brief Rule-of-3 Part 1: Copy constructor uses internal function clone().
     *
     */
    AvlTree(const AvlTree &rhs) : root{nullptr}
    {
        root = clone(rhs.root);
        size = rhs.size;
    }

    /**
     * @brief Rule-of-3 Part 2: Destroy the Binary Search Tree object using the internal
     *   function makeEmpty().
     */
    ~AvlTree()
    {
        makeEmpty();
    }

    /**
     * @brief Rule-of-3 Part 3: Copy assignment operator uses internal function clone().
     */
    AvlTree &operator=(const AvlTree &rhs)
    {
        makeEmpty();
        root = clone(rhs.root);
        size = rhs.size;
        return *this;
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty() const
    {
        return root == nullptr;
    }

    /**
     * Make the tree empty.
     */
    void makeEmpty()
    {
        makeEmpty(root);
    }

    /**
     * Return the number of elements inside of the tree.
    */
    size_t treeSize()
    {
        size_t s = privateTreeSize();
        return s;
    }

    /**
     * Insert a word with it's relative document ID and relevancy integer
     */
    void insert(const Key &x, const Value &v, const int &f)
    {
        insert(x, root, v, f);
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove(const Key &k)
    {
        remove(k, root);
    }

    /**
     * Find a specific word inside of the tree and return the vector of documents it's in.
    */
    map<Value, int> find(const Key &k)
    {
        return find(k, root);
    }

    /**
     * Traverse through the entire AVL Tree and save each node to a file.
    */
    void saveTree(ostream& out)
    {
        saveTreePrivate(out, root);
    }

private:
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert(const Key &k, AvlNode *&t, const Value &v, const int &f) // insert a template value into a specific key (insert a document into a vector under a specific word)
    {
        if (t == nullptr)
        {
            t = new AvlNode{k, nullptr, nullptr, 0, {}};
            t->valueAndRelevency[v] = f;
            size++;
            return; // a single node is always balanced
        }

        if (k < t->key) {
            insert(k, t->left, v, f);
        }
        else if (k > t->key) {
            insert(k, t->right, v, f);
        }
        else {
            t->valueAndRelevency[v] = f;
        } // Duplicate; do nothing

        // This will call balance on the way back up the tree. It will only balance
        // once at node the where the tree got imbalanced (called node alpha in the textbook)
        // and update the height all the way back up the tree.
        balance(t);
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove(const Key &k, AvlNode *&t)
    {
        if (t == nullptr) {
            return; // SPECIAL CASE: Do nothing...
        }

        if (k < t->key) {
            remove(k, t->left);
        }
        else if (k > t->key) {
            remove(k, t->right);
        }
        else {
            if (t->right == nullptr) {
                AvlNode* tempNode = t;
                t = t->left;
                delete tempNode;
                size = size - 1;
            }
            else {
                AvlNode* tempNode = t;
                std::pair<Key, Value> tempPair = removeHelper(t->right);
                tempNode->key = tempPair.first;
                tempNode->value = tempPair.second;
                size = size - 1;
            }
        }

        if (t != nullptr) {
            t->height = max(height(t->left), height(t->right)) + 1;
        }
        balance(t);
    }

    std::pair<Key, map<Value, int>> removeHelper(AvlNode* &n) {
        if (n == nullptr) { // SPECIAL CASE: Do nothing...
        }

        std::pair<Key, map<Value, int>> toReturn = {n->key, n->valueAndRelevency};

        if (n->left != nullptr) {
            toReturn = removeHelper(n->left);
        }
        else {
            AvlNode* tempNode = n;
            toReturn = {n->key, n->valueAndRelevency};
            n = n->right;
            delete tempNode;
            return toReturn;
        }

        if (n != nullptr) {
            n->height = max(height(n->left), height(n->right)) + 1;
        }
        balance(n);
        return toReturn;
    }

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty(AvlNode *&n)
    {
        if (n == nullptr)
            return;

        makeEmpty(n->left);
        makeEmpty(n->right);
        delete n;
        n = nullptr;
        size = 0;
    }

    /**
     * Internal method to find size of tree.
    */
    size_t privateTreeSize()
    {
        return size;
    }

    /**
     * Internal method to clone subtree.
     */
    AvlNode *clone(AvlNode *n) const
    {
        if (n == nullptr)
            return nullptr;

        return new AvlNode{n->key, clone(n->left), clone(n->right), n->height, n->valueAndRelevency};
    }

    /**
     * Internal method to find the vector of documents in a specific node.
    */
    map<Value, int> find(const Key &k, AvlNode* n) const
    {
        if (n == nullptr) {
            return map<Value, int>();
            // throw std::invalid_argument("Cannot find specified query in current index.");
        }

        if (k < n->key) {
            return find(k, n->left);
        }
        else if (k > n->key) {
            return find(k, n->right);
        }
        else {
            return n->valueAndRelevency;
        }
    }

    /**
     * Internal method to create the persistence.
    */
    void saveTreePrivate(ostream& out, AvlNode *n)
    {
        if (n == nullptr) {
            return;
        }

        saveTreePrivate(out, n->left);
        saveToFile(out, n->key, n->valueAndRelevency);
        saveTreePrivate(out, n->right);
    }

    void saveToFile(ostream& out, const Key &k, const map<Value, int> &v)
    {
        out << k << " ";
        for (auto& a : v) {
            out << a.first << " " << a.second << " ";
        }
        out << "\n";
    }

    // Balancing: AVL Rotations

    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height(AvlNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    static const int ALLOWED_IMBALANCE = 1; // 1 is the default; more will make balancing cheaper
                                            // but the search less efficient.

    /** 
     * 1. Performs rotations if the the the difference of the height stored in t's two child nodes 
     *    more than ALLOWED_IMBALANCE.
     * 2. Updates the height information of the note t.
     * 
     * Assumes that the high information in the child nodes is correct. This is guaranteed by calling
     * balance() recursivly from the inserted node up to the tree node (see insert()). Rotations will 
     * only be performed for node alpha (parent of the parent of the inserted node). For all other nodes, 
     * only the height will be updated. 
     */
    void balance(AvlNode *&t)
    {
        // special case: empty tree
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) // unbalancing insertion was left
        {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t); // case 1 (outside)
            else
                doubleWithLeftChild(t); // case 2 (inside)
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) // unbalancing insertion was right
        {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t); // case 4 (outside)
            else
                doubleWithRightChild(t); // case 3 (inside)
        }
        // else ... no imbalance was created

        // update height
        t->height = max(height(t->left), height(t->right)) + 1;
    }

    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild(AvlNode *&k2)
    {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild(AvlNode *&k1)
    {
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild(AvlNode *&k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild(AvlNode *&k1)
    {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }
};

#endif