#ifndef SLATE_MEM_H_
#define SLATE_MEM_H_

extern const struct mem_mod {
    void (*cpy)(void *dest, void *source);
    bool (*eql)(void *left, void *right);
} mem;

#endif
