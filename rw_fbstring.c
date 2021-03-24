#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "rw_fbstring.h"

int main(int argc, char *argv[])
{
//    xs string2;
//    xs* x = malloc(sizeof(xs));
    xs prefix = *xs_tmp("sssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaa"), suffix = *xs_tmp("xxxxxxx)))))))");
//    xs_concat(&x, &prefix, &suffix);
//    printf("[%s] : %2zu\n", xs_data(&x), xs_size(&x));
    xs string = *xs_tmp("\n foobarbar \n\n\n");
//    xs string3 = *xs_tmp("hello yo");
//    bool c = find_string(&string, "f");
//    printf("%B \n", c);
    xs_trim(&string, "\n ");
    printf("[%s] : %2zu\n", xs_data(&string), xs_size(&string));
//    fbstring* fb = fbstring_new(&string);
//    fb = fbstring_add(fb,&string);
    xs string1 = *xs_cow_copy_long(&string);
    printf("test on the stack \n");
    printf("[%s] : %2zu\n", xs_data(&string), xs_size(&string));
    printf("[%s] : %2zu\n", xs_data(&string1), xs_size(&string1));
    printf("%p : %p \n", &string.filler[0], &string1.filler[0]);
    xs string2 = *xs_tmp("Wayne is a smart, kind and friendly man ");
    xs string3 = *xs_cow_copy_long(&string2);
    printf("test on the heap \n");
    printf("[%s] : %2zu\n", xs_data(&string2), xs_size(&string2));
    printf("[%s] : %2zu\n", xs_data(&string3), xs_size(&string3));
    printf("%p : %p \n", &string2.ptr[0], &string3.ptr[0]);
    xs_concat(&string2, &prefix, &suffix);
//    xs string5 = *xs_tmp("sssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaaWayne is a smart, kind and friendly man ");
    xs string4 = *xs_cow_copy_long(&string2);
    printf("reference count %d \n", xs_get_refcnt(&string2) );
    printf("test is large string \n");
    printf("[%s] : %2zu\n", xs_data(&string2), xs_size(&string2));
    printf("[%s] : %2zu\n", xs_data(&string4), xs_size(&string4));
    printf("%p : %p \n", &(string2.ptr[0]), &(string4.ptr[0]));
    printf("reference count %d \n", xs_get_refcnt(&string2) );
    printf("reference count %d \n", xs_get_refcnt(&string4) );
    char *data = "aaaassssssssssssssssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaasssssssssssssssssssssssssssssssswwwwwwwwwwwwaaaaa";
    printf("%d \n", strlen(data));
    xs_cow_write_long(&string2, data);
    printf("[%s] : %2zu\n", xs_data(&string2), xs_size(&string2));
    printf("reference count %d \n", xs_get_refcnt(&string2) );
//    char *data = "foobarbar";
//    int c = xs_cow_lazy_copy(&string, &data);
//    printf("%s \n",&string.ptr);
//    printf("%s \n",data);
//    xs prefix = *xs_tmp("(((("), suffix = *xs_tmp(")))");
//    xs_concat(&string, &prefix, &suffix);
//    printf("[%s] : %2zu\n", xs_data(&string), xs_size(&string));
//    printf("[%s] : %2zu\n", xs_data(&string1), xs_size(&string1));
//    xs_concat(&string1, &prefix, &suffix);
//    printf("reference count : %d \n",xs_get_refcnt(&string1));
//    char* a = xs_cow_copy(&string);
//    printf("[%s] : %2zu\n", xs_data(&string1), xs_size(&string1));
//    printf("%s \n", a);
//    a[0] = 'd';
//    printf("%s \n", a);
}
