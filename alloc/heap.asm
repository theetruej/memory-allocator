bits 64;
global memspace
%define Heapsize (1024 * 1024 * 1024/4) ; Number of bytes in the heap (1GB)

Section .data 
    memspace:
        dq _memspace


Section .heap alloc noexec write nobits align=0x04
    _memspace:
        heapsize equ Heapsize
        resd heapsize

Section .note.GNU-stack noalloc
