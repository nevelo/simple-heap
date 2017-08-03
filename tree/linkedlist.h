/** Linked List Library.
 ** Written by Matt Saunders for CIS*2520 **/

#ifndef __MSAUND05_LINKEDLISTH
#define __MSAUND05_LINKEDLISTH

#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    void *data;
    struct ListNode *next;
};

struct List {
    struct ListNode *head;
    struct ListNode *tail;
    unsigned int length;
    void (*destFunc) (void *data);
    int (*compFunc) (void*, void*);
};

/**********************/
/** Public Functions **/
/**********************/

/* Allocates the memory for a new list.  Requires the user to pass it a function
 * to compare its data type, and a function to destroy its data type. */
struct List *newList(int (*__compare_function) (void*, void*), void (*__destroy_function) (void*));

/* Adds a piece of data to the list. */
void addToList(struct List *list, void *data);

/* Deallocates the memory for the list but NOT its data. Used if the data is
 * still being held elsewhere (like in a tree). */
void destroyListNotData(struct List *list);

/* Uses the given compare function to compare a given data with what's in the list.
 * If the data is found in the list, that node is removed from the list.
 * CURRENTLY NON-FUNCTIONAL. */
void *removeFromList(struct List *list, void *data);



/***********************/
/** Private Functions **/
/***********************/

/* Deallocates the memory for the node and its data. */
void killListNode(struct ListNode *node, void (*__destFunc) (void*));

/* Deallocates the memory for the node but NOT its data.  Used if the data is
 * still being held elsewhere (like in a tree). */
void killListNodeNotData(struct ListNode *node);

/* Allocates the memory for a new node. */
struct ListNode *newListNode(void *data);

/* Prints an entire list, given a function to print the type of data it holds. */
int printList(struct List *list, void (*__print_function) (void*) );

/* Removes a piece of data from a list.
 * CURRENTLY NON-FUNCTIONAL. */
void removeNodeFromList(struct List *list, struct ListNode *node);

#endif
