test:
	gcc -Wall -pedantic -std=c99 -static heaptest.c -L. -lheap -o test

library:
	gcc -c heap.c -o heap.o
	ar rcs libheap.a heap.o
	rm heap.o

shared-lib:
	gcc -c -fPIC heap.c -o heap.o
	gcc -shared -Wl,-soname,libheap.so.1 -o libheap.so.1.0.1 heap.o
	rm heap.o

test-shared:
	gcc -Wall -pedantic -std=c99 heaptest.c -o test-shared -L. -lheap
