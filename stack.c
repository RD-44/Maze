#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

static point* start;
static point* last;
static int size;

void createStack(int newSize){ //Size must be positive.
    start = last = calloc(sizeof(point), newSize);
    size = newSize;
}

void push(int x, int y, int prev){
    if(last < start + size) {(*last).x = x; (*last).y = y; (*last).prev = prev; last++;}
    else{printf("ERROR");}
}

point pop(){
    if(last > start){
        last--;
        return *(last);
    }else{
        point error;
        error.x = -1;
        return error;
    }
}

void clearStack(){
    free(start);
}