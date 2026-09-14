CC := cc
CFLAGS := -O2 -Wall -std=c23

alloc: alloc.o heap.o
	$(CC) $(CFLAGS) alloc.o heap.o -o alloc

alloc.o: alloc.c alloc.h
	$(CC) $(CFLAGS) -c alloc.c -o alloc.o

heap.o: heap.asm
	nasm -f elf64 heap.asm -o heap.o

clean:
	rm -f alloc alloc.o heap.o