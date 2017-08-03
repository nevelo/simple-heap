/** The AVL Tree Library.
 ** Written by Matt Saunders for CIS*2520 **/

#include "AVLtree.h"

/**********************
 ** Public functions **
 **********************/

void addToTree (AVLTree *tree, void *data) {
    AVLTreeNode *newNode;
    AVLTreeNode *newRoot;
    
    /* check that we're getting new data */
    if (data == NULL) {
        return;
    }
        
    /* check that the data isn't already in the tree */
    if (isInTree(tree, data)) {
        return;
    }
    
    /* make a new node for the data */
    newNode = createAVLNode(data);
    if (newNode == NULL) {
        return;
    }
    
    /* insert the new node into the tree */
    newRoot = insertAVLNode(tree->root, newNode, tree->compFunc);
    tree->root = newRoot;
    return;
}

AVLTree *createAVLTree(int (*__comparison_func) (void*, void*), void (*__destroy_func) (void*) ) {
    AVLTree *newTree = NULL;
    
    if (__comparison_func == NULL || __destroy_func == NULL)
        return NULL;
    
    newTree = malloc(sizeof(AVLTree));
    if (newTree == NULL)
        return NULL;
        
    newTree->root = NULL;
    newTree->compFunc = __comparison_func;
    newTree->destFunc = __destroy_func;
    
    return newTree;
}

void destroyAVLTree(AVLTree *tree) {
    if (tree == NULL) {
        return;
    }
    
    destroyAVLSubTree(tree->root, tree->destFunc);
    
    free(tree);
    return;
}

void *findInTree(AVLTree *tree, void *data) {
    AVLTreeNode *node;
    
    if (tree == NULL)
        return NULL;
        
    if (data == NULL)
        return NULL;
        
    node = findAVLNode(tree->root, data, tree->compFunc);
    if (node == NULL)
        return NULL;
        
    return node->data;
}

struct List *getValidDataList(AVLTree *tree, void *criteria, bool (*__validate_function) (void*, void*) ) {
    struct List *list;
    
    if (tree == NULL)
        return NULL; /* nonexistent tree */
    
    if (tree->root == NULL)
        return NULL; /* empty tree */
    
    list = newList(tree->compFunc, tree->destFunc);
    if (list == NULL)
        return NULL; /* malloc failure */
    
    populateList(tree->root, list, criteria, __validate_function);
    
    return list;
}

bool isInTree(AVLTree *tree, void *data) {
    AVLTreeNode *root; 
    int comp;
    
    if (tree == NULL || data == NULL)
        return FALSE; /* Data cannot be in tree if it doesn't exist -- or if tree doesn't exist */
    
    root = tree->root;
    while (root != NULL) {
        comp = tree->compFunc(root->data, data);
        if (comp == 0)
            return TRUE;
        if (comp < 0) /* root greater than data; going left */
            root = root->left;
        else /* root less than data, going right */
            root = root->right;
    }
    
    return FALSE;
}

void printInOrder(AVLTree *tree, void (*__print_function) (void *)) {
    if (tree == NULL)
        return;
        
    printWithoutSpaces(tree->root, __print_function);
    return;
}

void *removeFromTree (AVLTree *tree, void *data) {
    AVLTreeNode *foundData;
    void *toReturn;
    
    if (tree == NULL)
        return NULL;
        
    foundData = removeData(tree, tree->root, data, NULL, 0, tree->compFunc);
    if (foundData == NULL) {
        return NULL;
    }
    tree->root = balanceAVLTree(tree->root);
    toReturn = foundData->data;
    free(foundData);
    
    return toReturn;
}








/***********************
 ** Private functions **
 ***********************/

AVLTreeNode *balanceAVLTree(AVLTreeNode *root) {
    AVLTreeNode *newRoot = NULL;
    int lheight;
    int rheight;
    int balance;
    
    newRoot = root;
    
    if (root == NULL) {
        return root;
    }
    
    if (root->left == NULL)
        lheight = 0;
    else
        lheight = root->left->height;
        
    if (root->right == NULL)
        rheight = 0;
    else
        rheight = root->right->height;
    
    balance = lheight - rheight;
    if (balance > 1) { /* imbalanced, left-heavy */
        newRoot = rotRightAVL(root);
    } else if (balance < -1) { /* imbalanced, right-heavy */
        newRoot = rotLeftAVL(root);
    }
    
    return newRoot;
}

AVLTreeNode *createAVLNode(void *data) {
    AVLTreeNode *newNode = NULL;
    
    newNode = malloc(sizeof(AVLTreeNode));
    if (newNode == NULL)
        return NULL;
        
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    newNode->data = data;
    
    return newNode;
}

void destroyAVLSubTree(AVLTreeNode *root, void (*__destroy_func) (void*)) {
    if (root == NULL)
        return;
    
    __destroy_func(root->data);
    
    destroyAVLSubTree(root->left, __destroy_func);
    destroyAVLSubTree(root->right, __destroy_func);
    
    free(root);
    
    return;
}

/* Finds a node inside a tree and returns a pointer to it. */
AVLTreeNode *findAVLNode(AVLTreeNode *root, void *data, int (*__comparison_func) (void*, void*) ) {
    AVLTreeNode *foundNode;
    void *curData;
    int comp;
    
    if (root == NULL)
        return NULL; /* could not find data */
    curData = root->data;
    
    comp = __comparison_func(data, curData);
    if (comp == 0) { /* found the data */
        foundNode = root;
    } else if (comp > 0) { /* heading left */
        foundNode = findAVLNode(root->left, data, __comparison_func);
    } else { /* heading right */
        foundNode = findAVLNode(root->right, data, __comparison_func);
    }
    
    return foundNode;
}

AVLTreeNode *insertAVLNode(AVLTreeNode *root, AVLTreeNode *newNode, int (*__comparison_func) (void*, void*) ) {
    AVLTreeNode *newRoot;
    AVLTreeNode *newLeft;
    AVLTreeNode *newRight;
    int comp;
    
    newRoot = root;
    
    if (isAVLTreeEmpty(root)) {
        newNode->height = 1;
        return newNode; /* tree empty */
    }
    
    newLeft = root->left;
    newRight = root->right;
    
    if (newNode == NULL) {
        return root; /* new data nonexistent */
    }
    
    comp = __comparison_func(root->data, newNode->data);
    /* insertAVLnode() only gets called if the data is not a duplicate of data
     * already in the tree -- so no need to check for comp == 0. */
    
    if (comp > 0) { /* adding new data to right branch */
        if (newRight == NULL) { /* base case */
            newRight = newNode;
        } else {
            newRight = insertAVLNode(root->right, newNode, __comparison_func);
        }
    } else { /* comp < 0, adding new data to left branch */
        if (newLeft == NULL) {
            newLeft = newNode;
        } else {
            newLeft = insertAVLNode(root->left, newNode, __comparison_func);
        }
    }
    
    root->left = newLeft;
    root->right = newRight;
    recalcHeight(root);
    newRoot = balanceAVLTree(root);
    
    return newRoot;
}

bool isAVLTreeEmpty(AVLTreeNode *root) {
    if (root == NULL) {
        return TRUE;
    }
    
    return FALSE;
}

int max(int one, int two) {
    if (one > two) return one;
    else return two;
}

void populateList(AVLTreeNode *root, struct List *list, void *criteria, bool (*valFunc) (void*, void*) ) {
    
    if (root == NULL)
        return; /* reached rock bottom */
    
    populateList(root->left, list, criteria, valFunc);
    
    if ( valFunc(root->data, criteria) )
        addToList(list, root->data);
    
    populateList(root->right, list, criteria, valFunc);
    return;
}

void recalcHeight(AVLTreeNode *root) {
    int lh;
    int rh;
    
    if (root == NULL) return;
    
    if (root->left != NULL) {
        lh = root->left->height;
    } else lh = 0;
    
    if (root->right != NULL) {
        rh = root->right->height;
    } else rh = 0;
    
    root->height = max(lh, rh) + 1;
    return;
}

AVLTreeNode *removeData(AVLTree *tree, AVLTreeNode *root, void *data, AVLTreeNode *parent, int direc, int (*__compare_func) (void*, void*)) {
    AVLTreeNode *foundData;
    AVLTreeNode *nextLowest;
    void *thisLevelData;
    int comp;
    
    if (root == NULL)
        return NULL; /* Data does not exist in tree. */
        
    if (data == NULL)
        return NULL; /* Should never be triggered -- tried searching for nonexistent data. */
    
    thisLevelData = root->data;
    
    comp = __compare_func(data, thisLevelData);
    if (comp == 0) { /* Found the data! */
        /* Checking for edge cases (found node has one or zero branches) */
        foundData = root;
        if (root->right == NULL) { /* The data has no right node */
            if (direc == 1) { /* The found node is the parent's right child */
                parent->right = root->left;
            } else if (direc == -1) { /* The found node is the parent's left child */
                parent->left = root->left;
            } else { /* The found node is the parent of the entire tree */
                tree->root = root->left;
            }
        } else if (root->left == NULL) { /* the data has no left node */
            if (direc == 1) {
                parent->right = root->right;
            } else if (direc == -1) {
                parent->left = root->right;
            } else { /* the found node is the parent of the entire tree */
                tree->root = root->right;
            }
        } else { /* Need to traverse the tree to do the swap */
            nextLowest = removeNextLowest(root->left, root, -1); /* Can never return NULL from this function call -- that case is checked above */
            /* This function call returns the next lowest value from the subtree
             * rooted at the value we're deleting. Now that we have it, we can
             * substitute in the next lowest for the data we are removing.*/
            if (direc == 1) {
                parent->right = nextLowest;
            } else if (direc == -1) {
                parent->left = nextLowest;
            } else { /* the found node is the parent of the entire tree */
                tree->root = nextLowest;
            }
            nextLowest->left = foundData->left;
            nextLowest->right = foundData->right;
            recalcHeight(nextLowest);
        }
        foundData->left = NULL;
        foundData->right = NULL;
    } else if (comp > 0) { /* Need to head left -- didn't find the data */
        foundData = removeData(tree, root->left, data, root, -1, __compare_func);
    } else { /* Need to head right */
        foundData = removeData(tree, root->right, data, root, 1, __compare_func);
    }

    /* Recalculate height and rebalance */
    recalcHeight(root);
    if (foundData != root) {
        if (direc == -1) {
            parent->left = balanceAVLTree(root);
        } else if (direc == 1) {
            parent->right = balanceAVLTree(root);
        }
    }
    return foundData;
}

AVLTreeNode *removeNextLowest(AVLTreeNode *root, AVLTreeNode *parent, int direction) {
    AVLTreeNode *foundMax;
    
    /* Recursively finds max element in subtree by tracing right trees until no more right nodes. */
    /* Store parents so we can redirect pointers. */
    if (root == NULL)
        return NULL; /* Found the max. */
        
    foundMax = removeNextLowest(root->right, root, 1);
    
    if (foundMax == NULL) { /* We're at the maximum, so there's no root->right */
        foundMax = root;
        if (direction == 1) {
            parent->right = root->left;
        } else {
            parent->left = root->left;
        }
    } 
    recalcHeight(root);
    root = balanceAVLTree(root);
    return foundMax;
}

AVLTreeNode *rotLeftAVL(AVLTreeNode *root) {
    AVLTreeNode *newRoot;
    AVLTreeNode *oldRight; 
    AVLTreeNode *newRight;
    int rr; /* height of right right sub subtree */
    int rl; /* height of right left sub subtree */
    
    if (root == NULL) 
        return NULL;
    
    oldRight = root->right; /* root->right must exist for this subroutine to be called */
    
    if (oldRight->right != NULL) {
        rr = oldRight->right->height;
    } else rr = 0;
    
    if (oldRight->left != NULL) {
        rl = oldRight->left->height;
    } else rl = 0;
    
    if (rl > rr) { /* Right-Left Case, "oldRight" is the pivot */
        newRight = oldRight->left;
        oldRight->left = newRight->right;
        recalcHeight(oldRight);
        
        newRight->right = oldRight;
        recalcHeight(newRight);
        
        root->right = newRight;
    }
    
    newRoot = root->right;
    root->right = newRoot->left;
    
    recalcHeight(root);
    
    newRoot->left = root;
    recalcHeight(newRoot);
    
    return newRoot;
}

AVLTreeNode *rotRightAVL(AVLTreeNode *root) {
    AVLTreeNode *newRoot;
    AVLTreeNode *oldLeft; 
    AVLTreeNode *newLeft;
    int lr;
    int ll;
    
    if (root == NULL) 
        return NULL;
    
    oldLeft = root->left; /* root->left must exist for this subroutine to be called */
    
    if (oldLeft->right != NULL) {
        lr = oldLeft->right->height;
    } else lr = 0;
    
    if (oldLeft->left != NULL) {
        ll = oldLeft->left->height;
    } else ll = 0;
    
    if (lr > ll) { /* Left-Right Case, "oldLeft" is the pivot */
        newLeft = oldLeft->right;
        oldLeft->right = newLeft->left;
        recalcHeight(oldLeft);
        
        newLeft->left = oldLeft;
        recalcHeight(newLeft);
        
        root->left = newLeft;
    }
    
    newRoot = root->left;
    root->left = newRoot->right;
    
    recalcHeight(root);
    
    newRoot->right = root;
    recalcHeight(newRoot);
    
    return newRoot;
}

void printWithoutSpaces(AVLTreeNode *root, void (*__printFunc) (void*) ) {
    if (root == NULL)
        return;
        
    printWithoutSpaces(root->left, __printFunc);
    __printFunc(root->data);
    printWithoutSpaces(root->right, __printFunc);
    return;
}


/********************
 ** Test Functions **
 ********************/

void printOrderedTree(AVLTree *tree, void (*__printFunc) (void*) ) {    
    if (tree == NULL)
        return;
    
    printSubTree(tree->root, 0, 0, __printFunc);
    
    return;
}

void printSubTree(AVLTreeNode *root, int depth, int dir, void (*__printFunc) (void*) ) {
    int i;
    if (root == NULL)
        return;
    
    printSubTree(root->left, depth+1, 1, __printFunc);   
    for (i=0; i<depth; i++) {
        printf("    ");
    }
    if (dir == 1) printf("/- ");
    if (dir == -1) printf("\\- "); 
    printf("(h=%d)", root->height);
    __printFunc(root->data);
    printSubTree(root->right, depth+1, -1, __printFunc);
    return;
}

void printOrderedTreeAddresses(AVLTreeNode *root) {
    if (root == NULL)
        return;
    
    printOrderedTreeAddresses(root->left);
    printf("%p\n", (void*) root->data);
    printOrderedTreeAddresses(root->right);
    return;
}

