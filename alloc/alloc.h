#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>


typedef unsigned char int8;
typedef unsigned short int16;
typedef unsigned int int32;
typedef unsigned long long int64;
typedef unsigned __int128 int128;
typedef u_int8_t heap;
typedef int32 word;
#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))
#define Maxwords (1024 * 1024 * 1024 / 4) - 1 /* 1GB of memory in words */  

#define ErrNoMem 1

typedef struct packed s_header header;


struct packed s_header {
    word w: 30;
    bool allocated: 1;
    bool unused reserved: 1;
};


#define $1 (int8 *)
#define $2 (int16 *)
#define $4 (int32 *)
#define $8 (int64 *)
#define $16 (int128 *)
#define $c (char *)
#define $i (int)
#define $v (void *)
#define $h (header *)

#define reterr(x) errno = (x); return $v 0

/*allocate function*/
void *mkalloc(word, header*);
void *alloc(int32);
void read_mem(int8 *, word);
int main(int,char**);