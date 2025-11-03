// What's an AVL Tree: An AVL Tree is like an regular binary search tree (smaller values go left, bigger values go right)
// But it keeps itself balanced automatically, this means it never gets too lopsided, which keeps operations fast.

#include <stdio.h>
#include <stdlib.h>

struct root { int key; struct root *left, *right; };
typedef struct root root;

// functions that uses ref
void addNew(root **r, int key);
void removeNode(root **r, int key);
void findMaxRight(root **r);
void balanceTree(root **r);
void rotateRight(root **r);
void rotateLeft(root **r);

// functions that uses value
int sonsHeight(root *r);

void addNew(root **r, int key) {
    if (!*r) { // if root is null, insert new node
        *r = malloc(sizeof(**r));
        (*r)->key = key;
        (*r)->left = (*r)->right = NULL;
        return;
    }
    if (key < (*r)->key) addNew(&(*r)->left, key); // if key is smaller, goes left from the current node
    else if (key > (*r)->key) addNew(&(*r)->right, key); // if key is bigger, goes right from the current node

    balanceTree(r); // calls balance to adjust if needed, if (l - r = -1 \ 0 \ 1) ok!, else needs balance
}

void balanceTree(root **r) {
    if (!*r) return;

    root *aux;
    int balanceFactor = sonsHeight((*r)->right) - sonsHeight((*r)->left); // calculates balance from right and left
    
    if (balanceFactor <= -2) {
        aux = (*r)->left;
        // calculate balance from left son, if it's unbalanced, there's a need for double rotation
        balanceFactor = sonsHeight(aux->right) - sonsHeight(aux->left);

        if (balanceFactor > 0) { // means that the left side is also unbalanced, so there's a need for rotation as well
            rotateLeft(&(*r)->left); rotateRight(r);
            printf("\nDouble rotation right");
        }
        else {
            rotateRight(r);
            printf("\nSimple rotation right");
        }
    }
    else if (balanceFactor >= 2) {
        aux = (*r)->right;
        // calculate balance from right son, if it's unbalanced, tehre's a need for a double rotation
        balanceFactor = sonsHeight(aux->right) - sonsHeight(aux->left);

        if (balanceFactor < 0) { // means that the right side is also unbalanced, so there's a need for a rotation as well
            rotateRight(&(*r)->right); rotateLeft(r);
            printf("\nDouble rotation left");
        }
        else {
            rotateLeft(r);
            printf("\nSimple rotation left");
        }
    }
}

int sonsHeight(root *r) {
    if (r == NULL) return 0; // if node is null (does not exist, height is 0) so we return 0
    else {
        int heightLeft = sonsHeight(r->left); // calculate height of the left subtree
        int heightRight = sonsHeight(r->right); // calculate height of the right subtree
        return 1 + (heightLeft > heightRight ? heightLeft : heightRight); // returns 1 + maximum of the two heights
        // example, if i have a node with a left subtree of height 3 and a right subtree  of height 2, this node has height 4 (1 + 3)
    }
}

void rotateRight(root **r) {
    root *aux = (*r)->left; // aux turns into node left;
    aux->right = *r; // right from the aux goes to node
    (*r)->left = aux->right; // left from the node goes to aux right
    *r = aux; // root goes to aux
}

void rotateLeft(root **r) {
    root *aux = (*r)->right; // aux turns into node right
    aux->left = *r; // left from the aux goes to node
    (*r)->right = aux->left; // left from the node goes to aux right
    *r = aux; // root goes to aux
}
