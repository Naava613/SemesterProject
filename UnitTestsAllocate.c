#include <stdio.h>
#include <string.h>
//what to test:
// 1. check if the amount of space requested is now being used
//   This means that freeBlockSizes shouldn't that amount free, but it could be it does if another chunk has been broken down into that same amoutn of free space, so maybe you should just check firstPositions after and before and make sure the linkedList before is longer than the linkedList after?
//
int assertEqual(int expected, int actual);

int myMallocTest(); 
int works = 0; 
int myMallocTest(sizeRequested){
    char my_memory[20];
    mem_init(my_memory, 20);
    assertEqual(20, sizeof(my_memory));
    
    void* buffer = my_malloc(sizeRequested); 
    // create test char[] of size sizeRequested to store in that buffer.
    // Then read from buffer and ensure that the value read == the value written
    char hello[sizeRequested + 5];
    for (int i = 0; i < sizeRequested +  5; i++) {
        hello[i] = i;
    }
    buffer = hello;
    if (strcmp(hello, buffer) == 0) {
        return 0;
    }
    return 1;
    
}

int assertEqual(int expected, int actual) {
    if (expected == actual) {
        return 0;
    }
    return 1;
}

works = mem_init(20); //return one if successful

int my_malloc_16(){ 

}

int my_malloc_1(){

}