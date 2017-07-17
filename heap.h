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


#ifndef __MSAUND05_HEAPH
#define __MSAUND05_HEAPH

#include <stdlib.h>
#include <limits.h>
#include <stddef.h>

typedef struct heap Heap;

/* Creates a new heap, given an initial size.  The heap will need to be freed
   with a call to destroy_heap() after use. */
Heap *create_heap(size_t init_size);

/* Destroys an existing heap and all the data it contains. If your heap is 
   storing pointers to malloc'd data, pass a function that frees your data to
   __dest_func. If __dest_func is NULL, the data will not be freed. */
void destroy_heap(Heap *heap, void (*__dest_func) (void*));

/* Returns the value of the data with the lowest key. Sets the global variable
   LAST_KEY with the value associated with that key.  Does not remove from the 
   heap. If the heap is invalid, or nothing is currently present on the heap,
   LAST_KEY is set to the system's maximum integer value, INT_MAX, as set in
   the system header file limits.h. */
void *heap_peek(Heap *heap);

/* Returns the value of the data with the lowest key, and sets the global
   variable LAST_KEY with the value of the key.  Removes the data from the heap
   and performs a downheap to rebalance. */
void *heap_pop(Heap *heap);

/* Adds a new data and key pair to the heap, and performs an upheap to 
   rebalance. */
void *heap_push(Heap *heap, void *data, int key);

/* Returns the number of elements inside a heap. If the heap is not a valid
   heap, returns -1. */
size_t heap_get_size(Heap *heap);

#endif