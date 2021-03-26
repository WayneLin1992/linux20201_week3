#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "rw_fbstring.h"
int main(){
    FILE *fp = fopen("testbench1.txt", "r");
    FILE *fp1 = fopen("testbench2.txt", "r");
    FILE *fp2 = fopen("testbench3.txt", "r");
    FILE *fp3 = fopen("cxscow.txt", "w");
    char* str_stack = malloc(14*sizeof(char));
    char* str_heap = malloc(300*sizeof(char));
    char* str_b = malloc(13*sizeof(char));
    int i = 0;
    while (fgets(str_heap, 300, fp) != NULL && fgets(str_stack, 14, fp1) != NULL && fgets(str_b, 13, fp2) != NULL){
        struct timespec time_start;
        struct timespec time_end;
        long during = 0;
        fscanf(fp, "%[^\n]", str_heap);
        fscanf(fp1, "%[^\n]", str_stack);
        fscanf(fp2, "%[^\n]", str_b);
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        xs prefix = *xs_tmp(str_stack);
        xs string = *xs_tmp(str_heap);
        xs suffix = *xs_tmp(str_b);
        xs_concat(&string, &prefix, &suffix);
        xs_trim(&string, "b");
        xs string2 = *xs_cow_copy_long(&string);
        xs_cow_write_long(&string2, str_heap);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        i++;
        xs_free(&string);
        xs_free(&string2);
        xs_free(&prefix);
        xs_free(&suffix);
        during = time_end.tv_nsec - time_start.tv_nsec;
        printf("%d\t%ld \n",i,during);
        fprintf(fp3 ,"%d\t%ld \n",i,during);
    }
    free(str_b);
    free(str_stack);
    free(str_heap);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    return 0;
}

