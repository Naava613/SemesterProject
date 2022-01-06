#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AllocatePseudoCode.h"

//what to test:
// 1. check if the amount of space requested is now being used
//   This means that freeBlockSizes shouldn't that amount free, but it could be it does if another chunk has been broken down into that same amoutn of free space, so maybe you should just check MemoryPositionsStruct after and before and make sure the linkedList before is longer than the linkedList after?
//
int assertEqual(int expected, int actual);

int setUp(unsigned int memorySize);
int setUp(unsigned int memorySize){
    unsigned char my_memory[memorySize];
    mem_init(my_memory, memorySize);
    if (memorySize != sizeof(my_memory)) {
        return 1;
    }
}

int myMallocTest(unsigned int sizeRequested);

int myMallocTest(unsigned int sizeRequested){

    void* buffer = my_malloc(sizeRequested);
    // create test char[] of size sizeRequested to store in that buffer.
    // Then read from buffer and ensure that the value read == the value written
    char hello[sizeRequested];
    for (int i = 0; i < sizeRequested; i++) {
        hello[i] = i;
    }
    buffer = hello;
    if (strcmp(hello, buffer) == 0) {
        return 0;
    }
    return 1;
    
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

int removeNodesTest();

//get some already allocated space in my_memory and try to remove a node - in general
int removeNodesTest(){
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

    printf("it's null - general test!\n");
    free(nextNode);
    free(previousNode);
    return 0;
}

int removeBegNode();

//get some already allocated space in my_memory and try to remove a node from the beg
int removeBegNode(){
    //create a current node to pass into the function and check if it works
    struct BlockSizesStruct *currentNode = calloc(1, sizeof(struct BlockSizesStruct));
    struct BlockSizesStruct *nextNode = calloc(1, sizeof(struct BlockSizesStruct));
    struct BlockSizesStruct *previousNode = NULL;

    currentNode->nextNode = nextNode;
    currentNode->prevNode = previousNode;
    nextNode->prevNode = currentNode;
    removeNodeB(currentNode);
    //char pos = addressFirstPositions(struct oneBlockSizeStruct *currentNode);
    if (nextNode->prevNode != NULL) {
        return 1;
    }


    printf("it's null!\n");
    free(nextNode);
    return 0;
}

int removeLastNode();

//get some already allocated space in my_memory and try to remove a node from the beg
int removeLastNode(){
    //create a current node to pass into the function and check if it works
    struct BlockSizesStruct *currentNode = calloc(1, sizeof(struct BlockSizesStruct));
    struct BlockSizesStruct *nextNode = NULL;
    struct BlockSizesStruct *previousNode = calloc(1, sizeof(struct BlockSizesStruct));

    currentNode->nextNode = nextNode;
    currentNode->prevNode = previousNode;

    removeNodeB(currentNode);

    if (nextNode != NULL) {
        return 1;
    }

    free(nextNode);
    return 0;
}


int myMallocManyTimesBIG();
int myMallocManyTimesBIG(){
    setUp(50);
    myMallocTest(5);
    myMallocTest(3);
    myMallocTest(7);
}

int myMallocManyTimesSMALL();
int myMallocManyTimesSMALL(){
    setUp(10);
    myMallocTest(5);
    myMallocTest(3);
    myMallocTest(7);
}

int my_malloc_16(){
    setUp(20);
    myMallocTest(16);
    mem_stats_struct *memStatsStruct = calloc(1, sizeof(mem_stats_struct));
    mem_get_stats(memStatsStruct);
    if (memStatsStruct->num_blocks_free != 1) {
        return 1;
    }
    if (memStatsStruct->smallest_block_free != 4) {
        return 1;
    }
    if (memStatsStruct->largest_block_free != 4) {
        return 1;
    }
    if (memStatsStruct->num_blocks_used != 1) {
        return 1;
    }
    if (memStatsStruct->largest_block_used != 16) {
        return 1;
    }
    if (memStatsStruct->smallest_block_used != 16) {
        return 1;
    }
    return 0;
}

int my_malloc_1(){
    setUp(20);
    myMallocTest(1);
}

int main(void) {
    printf("Hello world!\n");

    if (my_malloc_1() != 0) {
       return -1;
    }
    printf("After my_malloc_1 test\n");
    if (my_malloc_16() != 0) {
        return -1;
    }
    printf("After my_malloc_16 test\n");
    if (addressFirstPositionsTest() != 0) {
        return -1;
    }
    printf("After addressFirstPositionsTest test\n");

    if (removeNodesTest() != 0){
        return -1;
    }
    if (removeBegNode() != 0){
        return -1;
    }

    if (removeLastNode() != 0){
        return -1;
    }

    printf("after all remove nodes tests\n");

    //test running my_malloc a few times and being able to insert into each one
    if (myMallocManyTimesSMALL() != 0){
        return -1;
    }
    if (myMallocManyTimesBIG() != 0){
        return -1;
    }
    printf("finished all tests!");
}
