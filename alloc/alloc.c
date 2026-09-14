/* alloc.c */
#include "alloc.h"
#include <stddef.h>
extern heap *memspace;

void *mkalloc(word words, header *hdr) {
    void *ret;
    word wordsin;
    ptrdiff_t bytesin;

    bytesin = ((unsigned char *)hdr - memspace);
    wordsin = (bytesin + words * sizeof(word)) / sizeof(word) + 1;
    if(words > (Maxwords-wordsin)) {
        return NULL;
    }
    hdr->w = words;
    hdr->allocated = true;
    ret = ($v hdr)+4;

    return ret;


}

void read_mem(int8 *buf, word bytes){
    if(buf == NULL){
        printf("Buffer is NULL\n");
        return;
    }

    for(word i = 0; i < bytes; i++){
        printf("%c", buf[i]);
    }
    printf("\n");
}

void write_mem(void *destination, const void *source, size_t bytes){
    int8 *dest = destination;
    const int8 *src = source;
    for(size_t i = 0; i < bytes; i++){
        dest[i] = src[i];
    }
}

void *alloc(int32 bytes){
    word words;
    void *mem;
    header *hdr;
    

    words = (!(bytes % 4))?
            bytes / 4 :
        (bytes / 4) + 1;
    mem = $v memspace;
    hdr = $h mem;
    if (!(hdr->w)) {
        if (words > Maxwords) {
            reterr(ErrNoMem);
        }
        mem = mkalloc(words,hdr);
        if(!mem){
            return NULL;
        }
        return mem;
    }
    return $v 0;
}

int main(int argc, char *argv[]) {
    int8 *p = alloc(10);
    char *ptr = "Hello, World!";
    write_mem(p, ptr, strlen(ptr));
    read_mem(p, strlen(ptr));
    

    return 0;
}


