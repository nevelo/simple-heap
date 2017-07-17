/* HEAPTEST.C: Programmed by Matt Saunders for CIS3110 Assignment 2. */

#include <stdio.h>
#include "./heap.h"

void print_entire_heap(Heap *heap);

int main(void) {
    Heap *heap;
        char test[12][12] = { "three", "eight", "five", "twelve", "one", "six", "four", "seven", "nine", "eleven", "ten", "two"};
    int keys[12] = {3,8,5,12,1,6,4,7,9,11,10,2};
    int i;
    
    printf("Heap test\n");
    heap = create_heap(5);
    printf("Created heap at memory address %lX\n", (unsigned long int) heap);

    for(i=0; i<12; i++) {
        printf("Pushing %d: %s\n", keys[i], test[i]);
        heap_push(heap, (void*) test[i], keys[i]);
        printf("C: ");
        print_entire_heap(heap);
        printf("Last key = %d\n", heap_get_last_key(heap));
        printf("\n");
    }

    while (heap_get_size(heap) > 0) {
        printf("Popping... returned %s, ", (char*) heap_pop(heap));
        printf("last key = %d\n", heap_get_last_key(heap));
    }

    destroy_heap(heap, NULL);
    printf("Destroyed heap.\n");
    return 0;
}


/* Paste in heap.c to test:

#include <stdio.h>
void print_entire_heap(Heap *heap) {
    for (int i=0; i<heap_get_size(heap); i++) {
        printf("%d: %s | ", i, (char*) ((heap->root)+i)->data );
    }
}

*/