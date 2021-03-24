#include <stdint.h>
#include "intern.h"

fbstring* fbstring_new(xs* string){
    fbstring *fb = malloc(sizeof(fbstring));
    fb->size = 0;
    fb->total = 1023 - fb->size;
    fb->pool.node[fb->size].refcnt = 1;
    fb->pool.node[fb->size].x = string;

    return fb;
}

//fbstring* fbstring_add(fbstring* fb,xs* string){
//    for(int i=0; i = fb->size ; i++){
//      if(string->is_ptr){
//        int j = strcmp(fb->pool.node[i].x.ptr,string->ptr);
//        if(j==1)
//            break;
//      }
//    }
//        fb->size++;
//        fb->total--;
//        fb->pool.node[fb->size].refcnt = 1;
//        fb->pool.node[fb->size].x = string;
//        return fb;
//}
