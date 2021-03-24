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
    FILE *fp1 = fopen("cstringcd.txt", "w");
    if (!fp) {
        perror("testbench1.txt");
        exit(EXIT_FAILURE);
    }
    int  i  =0;
    //char* str = malloc(300*sizeof(char));
    //fprintf(fp1,"count\tlist_sort(merge)\n");
    for( i=0; i<50; i++){
        struct timespec time_start;
        struct timespec time_end;
        long during = 0;
        char* str = malloc(300*sizeof(char));
        fscanf(fp, "%[^\n]", str);
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        xs string = *xs_tmp(str);
        xs_free(&string);
        //printf("Data from the file:\n%s", str);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        during = time_end.tv_nsec - time_start.tv_nsec;
        fprintf(fp1 ,"%d\t%ld \n",i,during);
    }
    fclose(fp);
    fclose(fp1);
    return 0;
}
