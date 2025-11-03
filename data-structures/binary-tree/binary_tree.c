#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct root { int key; struct root *left, *right; };
typedef struct root root;

#define RANDOM_CAP 40
#define NUMBER_GEN_CAP 10

// modifies the tree (passed by ref)
void addNewRoot(root **r, int key);
void removeRoot(root **r, int key);

// don't change the tree (passed by val)
void printPreOrder(root *r);
void printInOrder(root *r);
void printPosOrder(root *r);
root *searchRoot(root *r, int key);
root *findMaxLeft(root **r);

int main() {
    srand(time(NULL)); // seed with current time
    root *root = NULL; int selectedNums[NUMBER_GEN_CAP] = {0};

    for (int i = 0; i < NUMBER_GEN_CAP; i++) {
        int randomNumber = (rand() % RANDOM_CAP) + 1;
        addNewRoot(&root, randomNumber); selectedNums[i] = randomNumber;
    }
    
    printf("\n---- print in order ----\n");
    printInOrder(root);
    printf("\n---- print pre order ----\n");
    printPreOrder(root);
    printf("\n---- print pos order ----\n");
    printPosOrder(root);

    for (int i = 0; i < 3; i++) {
        int randomNumber = (rand() % NUMBER_GEN_CAP);
        removeRoot(&root, selectedNums[randomNumber]);
    }

    printf("\n---- print in order ----\n");
    printInOrder(root);
    printf("\n---- print pre order ----\n");
    printPreOrder(root);
    printf("\n---- print pos order ----\n");
    printPosOrder(root);

    return 0;
}

// ---- functions passed by ref ----
void addNewRoot(root **r, int key) {
    if (!*r) { // means that the root it's currently in the spot of adding a new root
        *r = malloc(sizeof(**r));
        if (!*r) return;

        (*r)->key = key;
        (*r)->left = (*r)->right = NULL;
        return; // Node allocated, safely returns from the function
    }

    // calls recursion on the stack till it's on a null space
    if (key < (*r)->key) addNewRoot(&(*r)->left, key); // goes to left side of the curr root
    else if (key > (*r)->key) addNewRoot(&(*r)->right, key); // goes to right side of the curr root
}

void removeRoot(root **r, int key) {
    if (!*r) return; // means that there's no node to remove, so return from the function
    if (key < (*r)->key) removeRoot(&(*r)->left, key); // if target key is smaller, goes left from the curr node
    else if (key > (*r)->key) removeRoot(&(*r)->right, key); // if target key is bigger, goes right from the curr node
    else {
        root *aux = *r;
        
        if (aux->left == NULL && aux->right == NULL) { free(aux), (*r) = NULL; } // if target node has no sons
        else {
            // if father has only right son, son takes place of father and free father after adjusting right pointer
            if (aux->left == NULL) { (*r) = aux->right; aux->right = NULL; free(aux); }
            // if father has only left son, son takes place of father and free father after adjusting left pointer
            else if (aux->right == NULL) { (*r) = aux->left; aux->left = NULL; free(aux); }
            else {
                aux = findMaxLeft(&(*r)->left); // finds rightmost and returns/removes from it's original place
                aux->left = (*r)->left; aux->right = (*r)->right; // assigns current root's left/right to new node
                free(*r); // free current root
                *r = aux; // assigns it's place to new root
            }
        }
    }
}

// ---- functions passed by val ----
void printPreOrder(root *r) {
    if (!r) return; // currently root is empty so no value to print
    printf("%d ", r->key); // prints father, then goes to sons (F -> S1 -> S2)
    printPreOrder(r->left); printPreOrder(r->right); // calls again function and stacks on top the call before, until it's null
}

void printInOrder(root *r) {
    if (!r) return; // currently root is empty so no value to print
    printInOrder(r->left); // goes to son on the left first (S1 -> F -> S2)
    printf("%d ", r->key);
    printInOrder(r->right); // lastly goes to son on the right and prints
}

void printPosOrder(root *r) {
    if (!r) return; // currently root is empty so no value to print
    printPosOrder(r->left); // goes to son o the left first (S1 -> S2 -> F)
    printPosOrder(r->right); // goes to son o the right secondly 
    printf("%d ", r->key); // prints current root
}

root *searchRoot(root *r, int key) {
    if (!r || r->key == key) return r; // returns null or r
    return (key > r->key) ? searchRoot(r->right, key) : searchRoot(r->left, key); // calls in stack, and if found returns
}

root *findMaxLeft(root **r) {
    if ((*r)->right) return findMaxLeft(&(*r)->right); // goes to rightmost node from the left side of the tree
    else {
        root *aux = *r; // allocates an aux to mark ref to rightmost node
        if (aux->left) *r = aux->left; // checks if the rightmost node has a left child, if there's, he takes place of father
        else *r = NULL;  // if there's no son o left, just sets it to null
        return aux; // return rightmost node, after setting that place as null, completing removal
    }
}
