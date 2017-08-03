/** Linked List Library.
 ** Written by Matt Saunders for CIS*2520 **/

#include "linkedlist.h"

struct List *newList(int (*__compare_function) (void*, void*), void (*__destroy_function) (void*)) {
    struct List *list = NULL;
    
    list = malloc(sizeof(struct List));
    if (list == NULL) {
        return NULL;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->destFunc = __destroy_function;
    list->compFunc = __compare_function;
    
    return list;
}

void addToList(struct List *list, void *data) {
    struct ListNode *new;
    
    if (list == NULL)
        return; /* cannot add to null list */
    if (data == NULL)
        return; /* cannot add no data to list */
    
    new = newListNode(data);
    if (new == NULL)
        return; /* failed to allocate memory */
    
    if (list->head == NULL) {
        list->head = new;
        list->tail = new;
    } else {
        list->tail->next = new;
        list->tail = new;
    }
        
    return;
}

void destroyListNotData(struct List *list) {
    struct ListNode *cur;
    struct ListNode *next;
    
    if (list == NULL)
        return;
        
    cur = list->head;
    
    while (cur != NULL) {
        next = cur->next;
        killListNodeNotData(cur);
        cur = next;
    }
    
    free(list);
    return;
}

int printList(struct List *list, void (*__print_function) (void*) ) {
    struct ListNode *cur;
    int num = 0;
    
    if (list == NULL)
        return 0;
        
    cur = list->head;
    while (cur != NULL) {
        __print_function(cur->data);
        cur = cur->next;
        num++;
    }
    
    return num;
}

void *removeFromList(struct List *list, void *data) {
    void *toReturn = NULL;
    
    return toReturn;
}



/***********************/
/** Private Functions **/
/***********************/

void killListNode(struct ListNode *node, void (*destFunc) (void*)) {
    if (node == NULL)
        return;
        
    destFunc(node->data);
    free(node);
    return;
}
 
void killListNodeNotData(struct ListNode *node) {
    free(node);
    return;
}

struct ListNode *newListNode(void *data) {
    struct ListNode *node;
    
    node = malloc(sizeof(struct ListNode));
    if (node == NULL)
        return NULL;
        
    node->data = data;
    node->next = NULL;
    return node;
}

void removeNodeFromList(struct List *list, struct ListNode *node) {

    return;
}
