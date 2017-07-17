test:
	gcc -Wall -pedantic -std=c99 -static heaptest.c -L. -lheap -o test

library:
	gcc -c heap.c -o heap.o
	ar rcs libheap.a heap.o
	rm heap.o
