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

void dealloc(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    header *hdr = $h((unsigned char *)ptr - sizeof(header));
    hdr->allocated = false;
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
    if (!hdr->allocated) {
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

void reallocate(void **ptr, int32 new_size) {
    if(ptr == NULL) {
        alloc(new_size);
        return;
    }
    if (*ptr == NULL) {
        *ptr = alloc(new_size);
        return;
    }
    if (new_size == 0) {
        dealloc(*ptr);
        *ptr = NULL;
        return;
    }
    header *hdr = $h((unsigned char *)*ptr - sizeof(header));
    int32 old_size = hdr->w * sizeof(word);
    if (new_size <= old_size) {
        return;
    }
    void *new_ptr = alloc(new_size);
    if (new_ptr == NULL) {
        return;
    }
    write_mem(new_ptr, *ptr, old_size);
    dealloc(*ptr);
    *ptr = new_ptr;
}

int main(int argc, char *argv[]) {
    char *ptr = "Hello, World!";
    int8 *p = alloc(strlen(ptr) + 1);
    write_mem(p, ptr, strlen(ptr));
    read_mem(p, strlen(ptr));
    dealloc(p);
    p = NULL;
    printf("Memory deallocated successfully\n");
    char *new_ptr = "New allocation!";
    p = alloc(strlen(new_ptr) + 1);
    if (p == NULL) {
        return 1;
    }
    write_mem(p, new_ptr, strlen(new_ptr));
    read_mem(p, strlen(new_ptr));
    reallocate((void**)&p, 30);
    if (p == NULL) {
        return 1;
    }
    char *realloc_ptr = "Reallocated memory!";
    write_mem(p, realloc_ptr, strlen(realloc_ptr));
    read_mem(p, strlen(realloc_ptr));

    return 0;
}


