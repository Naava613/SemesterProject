#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AllocatePseudoCode.h"

//what to test:
// 1. check if the amount of space requested is now being used
//   This means that freeBlockSizes shouldn't that amount free, but it could be it does if another chunk has been broken down into that same amoutn of free space, so maybe you should just check MemoryPositionsStruct after and before and make sure the linkedList before is longer than the linkedList after?
//
int assertEqual(int expected, int actual);

int myMallocTest(); 
//int works = 0; 
int myMallocTest(unsigned int sizeRequested){
    unsigned char my_memory[20];
    mem_init(my_memory, 20);
    if (20 != sizeof(my_memory)) {
        return 1;
    } 
    
    //void* buffer = my_malloc(sizeRequested);
    // create test char[] of size sizeRequested to store in that buffer.
    // Then read from buffer and ensure that the value read == the value written
    char hello[sizeRequested];
    for (int i = 0; i < sizeRequested; i++) {
        hello[i] = i;
    }
    //buffer = hello;
    //if (strcmp(hello, buffer) == 0) {
    //    return 0;
    //}
    return 0;
    
}

int addressFirstPositionsTest();
int addressFirstPositionsTest() {
    unsigned char my_memory[20];
    mem_init(my_memory, 20);
    struct BlockSizesStruct *returnValue = addressFirstPositions(5, 1); //return memory location
    // (check that it's the yellow A)
    if (returnValue == NULL) {
        return 1;
    }
    printf("%x", returnValue->addressFP);
    return 0;
}

int removeNodesTestBeg();

//get some already allocated space in my_memory and try to remove a node from the beg, middle, and end
int removeNodesTestBeg(){
    //create a current node to pass into the function and check if it works
    struct BlockSizesStruct *currentNode = calloc(1, sizeof(struct BlockSizesStruct));
    struct BlockSizesStruct *nextNode = calloc(1, sizeof(struct BlockSizesStruct));
    struct BlockSizesStruct *previousNode = calloc(1, sizeof(struct BlockSizesStruct));

    currentNode->nextNode = nextNode;
    currentNode->prevNode = previousNode;
    nextNode->prevNode = currentNode;
    previousNode->nextNode = currentNode;
    removeNodeB(currentNode);
    //char pos = addressFirstPositions(struct oneBlockSizeStruct *currentNode);
    if (nextNode->prevNode != previousNode) {
        return 1;
    }
    if (previousNode->nextNode != nextNode) {
        return 1;
    }

    printf("it's null!\n");
    free(nextNode);
    free(previousNode);
    free(currentNode); //
    return 0;
}

int addAllocatedTest() {

}

//works = mem_init(20); //return one if successful

int my_malloc_16(){ 
    myMallocTest(16);
}

int my_malloc_1(){
    myMallocTest(1);
}

int main(void) {
    printf("Hello world!\n");
    //if (my_malloc_1() != 0) {
    //   return -1;
    //}
    printf("After my_malloc_1 test\n");
    //if (my_malloc_16() != 0) {
    //    return -1;
    //}    printf("After my_malloc_16 test\n");
    if (addressFirstPositionsTest() != 0) {
        return -1;
    }
    printf("After addressFirstPositionsTest test\n");

}
