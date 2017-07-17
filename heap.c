/* HEAP: a heap library in C. Copyright (C) 2016, 2017 Matthew Saunders.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

   To contact the author of this software, please e-mail:

        msaund05@mail.uoguelph.ca
*/

#include <stddef.h>

struct __heapnode {
    int key;
    void *data;
};

struct heap {
    struct __heapnode *root;
    size_t size;
    size_t alloc_size;
    size_t init_size;
};

#include "heap.h"

/* Internal functions */



void destroy_heapnode(struct __heapnode *node, void (*__dest_func) (void*)) {
    if (node == NULL) return;

    if (__dest_func != NULL) {
        __dest_func(node->data);
    }
    return;
}

int increase_heap_size(Heap *heap) {
    struct __heapnode *new_block;

    if (heap != NULL) {
        new_block = realloc(heap->root, sizeof(struct __heapnode) * (heap->alloc_size + heap->init_size));
        if (new_block != NULL) {
            heap->root = new_block;
            heap->alloc_size = heap->alloc_size + heap->init_size;
            return 0;
        }
        return 1; 
    }
    return -1;
}

void heapnode_swap(struct __heapnode *one, struct __heapnode *two) {
    int key;
    void *data;

    data = one->data;
    key = one->key;
    one->data = two->data;
    one->key = two->key;
    two->data = data;
    two->key = key;
}

void upheap(Heap *heap, int pos) {
    int parent;
    struct __heapnode *root;
    if (heap == NULL) return;
    if (pos == 0) return;
    
    root = heap->root;
    parent = ( pos + (pos % 2) - 2 ) / 2;
    if (root[parent].key < root[pos].key) return;
    heapnode_swap(&(root[parent]), &(root[pos]));
    upheap(heap, parent);
}

void downheap(Heap *heap, int pos) {
    int lchild;
    int rchild;
    int last_element;

    if (heap == NULL) return;
    last_element = heap->size - 1;
    lchild = 2 * pos + 1;
    rchild = 2 * pos + 2;
    if (lchild > last_element) return; /* Has no children. */

    if ( rchild > last_element || ( (heap->root)[lchild].key < (heap->root)[rchild].key ) ) {

        if ( (heap->root)[lchild].key < (heap->root)[pos].key ) {
            heapnode_swap(&((heap->root)[lchild]), &((heap->root)[pos]));
            downheap(heap, lchild);
            return;
        }

        if (rchild > last_element) return;
        if ( (heap->root)[rchild].key < (heap->root)[pos].key ) {
            heapnode_swap( &((heap->root)[rchild]), &((heap->root)[pos]));
            downheap(heap, rchild);
            return;
        }
    } else { 
        if ( (heap->root)[rchild].key < (heap->root)[pos].key ) {
            heapnode_swap( &((heap->root)[rchild]), &((heap->root)[pos]));
            downheap(heap, rchild);
            return;
        }  
    }

    return; /* Node is in its proper spot. */
}

#include <stdio.h>
void print_entire_heap(Heap *heap) {
    for (int i=0; i<heap_get_size(heap); i++) {
        printf("%d: %s | ", i, (char*) ((heap->root)+i)->data );
    }
}

/* External functions */

Heap *create_heap(size_t init_size){
    Heap *new;
    
    if (init_size < 1) return NULL;
    new = malloc(sizeof(Heap));
    if (new != NULL) {
        new->root = malloc(sizeof(struct __heapnode) * init_size);
        new->size = 0;
        new->alloc_size = new->init_size = init_size;
    }

    return new;
}

void destroy_heap(Heap *heap, void (*__dest_func) (void*)) {
    size_t i;

    if (heap == NULL) return;
    for (i=0; i<heap->size; i++) {
        destroy_heapnode(&((heap->root)[i]), __dest_func);
    }
    free(heap->root);
    free(heap);
}

void *heap_peek(Heap *heap) {
    LAST_KEY = INT_MAX;
    if (heap == NULL) return NULL;
    if (heap->root == NULL) return NULL;
    LAST_KEY = (heap->root)[0].key;
    return (heap->root)[0].data;
}

void *heap_pop(Heap *heap) {
    void *data;

    LAST_KEY = INT_MAX;
    if (heap == NULL) return NULL;
    if (heap->size == 0) return NULL;

    heap->size = heap->size - 1;
    if (heap->size != 0) {
        heapnode_swap( &((heap->root)[0]), &((heap->root)[heap->size]) );
    }

    LAST_KEY = (heap->root)[heap->size].key;
    data = heap->root[heap->size].data;

    downheap(heap, 0);
    return data;
}

void *heap_push(Heap *heap, void *data, int key) {
    int new_position;

    if (heap == NULL) return NULL;

    new_position = heap->size;
    while (heap->size >= heap->alloc_size) {
        if (increase_heap_size(heap) != 0) {
            return NULL;
        }
    }
    (heap->root)[new_position].data = data;
    (heap->root)[new_position].key = key;
    heap->size = heap->size + 1;
    upheap(heap, new_position);
    return heap;
}

size_t heap_get_size(Heap *heap) {
    if (heap == NULL) return -1;
    return heap->size;
}