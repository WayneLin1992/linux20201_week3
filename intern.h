#ifndef INTERN_H_INCLUDED
#define INTERN_H_INCLUDED

#include "rw_fbstring.h"
//#include "intern.c"

#define INTERNING_POOL_SIZE 1024
//struct __cstr_node {
//    char buffer[CSTR_INTERNING_SIZE];
//    struct __cstr_data str;
//    struct __cstr_node *next;
//};

struct __fbstring_node{
    xs* x;
    int refcnt;
    struct __fbstring_node *next;
};

struct __fbstring_pool {
    struct __fbstring_node node[INTERNING_POOL_SIZE];
};

struct __fbstring_interning {
    int lock;
    int index;
    int size;
    int total;
    //struct __cstr_node **hash;
    struct __fbstring_pool pool;
};

typedef struct __fbstring_interning  fbstring;

fbstring* fbstring_new(xs* string);//{
//    fbstring *fb = malloc(sizeof(fbstring));
//    fb->size = 0;
//    fb->total = 1023 - fb->size;
//    fb->pool.node[fb->size].refcnt = 1;
//    fb->pool.node[fb->size].x = string;
//
//    return fb;
//}
fbstring* fbstring_add(fbstring* fb,xs* string);
#endif // INTERN_H_INCLUDED
