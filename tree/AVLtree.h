/** AVL Tree Library
 ** Written by Matt Saunders for CIS*2520 **/

#ifndef __MSAUND05_AVLTREEH
#define __MSAUND05_AVLTREEH

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

typedef enum bool {
    FALSE,
    TRUE
} bool;

typedef struct AVLTreeNode {
    struct AVLTreeNode *left;
    struct AVLTreeNode *right;
    void *data;
    int height;
} AVLTreeNode;

typedef struct AVLTree {
    AVLTreeNode *root;
    void (*destFunc) (void *data);
    int (*compFunc) (void*, void*);
} AVLTree;

/** Public Functions **/

/* Adds a data pointer to the tree.  Rebalances the tree after addition. */
void addToTree (AVLTree *tree, void *data);

/* Creates a new AVL Tree.  Requires a comparison function and a destruction function
 * for the type of data being held in the tree. */
AVLTree *createAVLTree(int (*__comparison_func) (void*, void*), void (*__destroy_func) (void*) );

/* Destroys an AVL tree.  Frees all of the data inside the tree recursively. */
void destroyAVLTree(AVLTree *tree);

/* Finds a piece of data in an AVL tree. Returns a pointer to the data.  Leaves
 * the data in place in the tree. Returns NULL if the data is not found. */
void *findInTree(AVLTree *tree, void *data);

/* Steps through a tree, sending each data element into the validation function.
 * If the piece of data fits the criteria, it is added to a list which is created
 * by this function.  The data is NOT copied when added to the list; the list
 * MUST be freed with the destroyListNotData() function!
 * 
 * The criteria is given by the user and passed into the validate function.
 * 
 * The validation function MUST have the prototype:
 * bool __validate_function(void *data, void *criteria);
 * It MUST return TRUE if the data fits the criteria, or FALSE if not. */
struct List *getValidDataList(AVLTree *tree, void *criteria, bool (*__validate_function) (void*, void*) );

/* Checks if a piece of data is inside a tree, using its previously-defined comparison function. */
bool isInTree(AVLTree *tree, void *data);

/* Prints a tree in order. Requires a function capable of printing a node. */
void printInOrder(AVLTree *tree, void (*__print_function) (void *));

/* Removes a piece of data from a tree.  Rebalances the tree after deletion.
 * Returns NULL if file not found. */
void *removeFromTree (AVLTree *tree, void *data);



/** Private functions **/

/* Balances a given subtree.  Returns the new root. */
AVLTreeNode *balanceAVLTree(AVLTreeNode *root);

/* Allocates the memory for a new node. */
AVLTreeNode *createAVLNode(void *data);

/* Given a subtree and a destroy function, recursively destroys each tree node
 * and the data inside of it. */
void destroyAVLSubTree(AVLTreeNode *root, void (*__destroy_func) (void*));

/* Finds a node inside a tree and returns a pointer to it. */
AVLTreeNode *findAVLNode(AVLTreeNode *root, void *data, int (*__comparison_func) (void*, void*) );

/* Inserts a node inside a tree and returns a pointer to the new root.
 * Must only be called if we have already determined new data is not in tree. */
AVLTreeNode *insertAVLNode(AVLTreeNode *root, AVLTreeNode *newNode, int (*__comparison_func) (void*, void*) );

/* Checks if a subtree has any children. Returns TRUE if empty, FALSE if not. */
bool isAVLTreeEmpty (AVLTreeNode *root);

/* Takes two ints, returns their maximum */
int max(int one, int two);

/* Recursively steps through the tree to populate the list IN-ORDER.  Uses the
 * given valFunc() to determine if the tree member should be added to the list.
 * The criteria is given by the user and passed into the validation function. */
void populateList (AVLTreeNode *root, struct List *list, void *criteria, bool (*valFunc) (void*, void*) );

/* Given a root node, recalculates its height based on its branch nodes + 1 */
void recalcHeight(AVLTreeNode *root);

/* Removes a piece of data from the tree, rebalancing the tree in the process. 
 * Direc tells the function the direction that the parent traversed.  -1 for left, 1 for right.
 * 0 is a special value denoting the root of the entire tree.*/
AVLTreeNode *removeData(AVLTree *tree, AVLTreeNode *root, void *data, AVLTreeNode *parent, int direc, int (*__compare_func) (void*, void*));

/* Finds the next lowest value of a particular value, removes it from the tree, and returns it. 
 * Direction can be -1 for left, 1 for right. */
AVLTreeNode *removeNextLowest(AVLTreeNode *root, AVLTreeNode *parent, int direction);

/* Rotates a given subtree left.  Returns the new root. */
AVLTreeNode *rotLeftAVL(AVLTreeNode *root);

/* Rotates a given subtree right.  Returns the new root. */
AVLTreeNode *rotRightAVL(AVLTreeNode *root);

/* Prints a subtree recursively, in order. */
void printWithoutSpaces(AVLTreeNode *root, void (*__printFunc) (void*) );

/** Test Functions **/

void printOrderedTree(AVLTree *tree, void (*__printFunc) (void*) );
void printSubTree(AVLTreeNode *root, int depth, int dir, void (*__printFunc) (void*) );
void printOrderedTreeAddresses(AVLTreeNode *root);


#endif
