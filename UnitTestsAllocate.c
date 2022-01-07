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

int mem_stats_check(int expected_num_blocks_used, int expected_num_blocks_free, int expected_smallest_block_free,
                    int expected_smallest_block_used, int expected_largest_block_free, int expected_largest_block_used) {
    mem_stats_struct *memStatsStruct = calloc(1, sizeof(mem_stats_struct));
    mem_get_stats(memStatsStruct);
    if (memStatsStruct->num_blocks_free != expected_num_blocks_free) {
        return 1;
    }
    if (memStatsStruct->smallest_block_free != expected_smallest_block_free) {
        return 1;
    }
    if (memStatsStruct->largest_block_free != expected_largest_block_free) {
        return 1;
    }
    if (memStatsStruct->num_blocks_used != expected_num_blocks_used) {
        return 1;
    }
    if (memStatsStruct->largest_block_used != expected_largest_block_used) {
        return 1;
    }
    if (memStatsStruct->smallest_block_used != expected_smallest_block_used) {
        return 1;
    }
    return 0;
}
void print_stats(char *prefix);
void print_stats(char *prefix) {

    mem_stats_struct mem_stats;

    mem_get_stats(&mem_stats);

    printf("mem stats: %s: %d free blocks, %d used blocks, free blocks: smallest=%d largest=%d, used blocks: smallest=%d largest=%d\n",

           prefix,

           mem_stats.num_blocks_free,

           mem_stats.num_blocks_used,

           mem_stats.smallest_block_free,

           mem_stats.largest_block_free,

           mem_stats.smallest_block_used,

           mem_stats.largest_block_used);

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
    mem_stats_check(1, 1, 4, 16, 4, 16);
    return 0;
}

int my_freeTest(){
    unsigned char my_memory[50];
    mem_init(my_memory, 50);
    void *firstAddress = my_malloc(10);
    mem_stats_check(1, 1, 40, 10, 40, 10);
    void *SecondAddress = my_malloc(7);
    int status = mem_stats_check(2, 1, 33, 7, 33, 10);
    printf("stats: %d\n", status);

    my_free(firstAddress);
    status = mem_stats_check(1, 2, 10, 7, 33, 7);
    printf("stat after my free: %d\n", status);
    my_malloc(20);
    my_free(SecondAddress);
    status = mem_stats_check(1, 2, 13, 20, 17, 20);
    printf("status after another free is called: %d\n", status);
    return 0;
}

int my_freeTest2(){
    unsigned char my_memory[50];
    mem_init(my_memory, 50);
    void *firstAddress = my_malloc(10);
    mem_stats_check(1, 1, 40, 10, 40, 10);
    void *SecondAddress = my_malloc(7);
    int status = mem_stats_check(2, 1, 33, 7, 33, 10);
    printf("2 stats: %d\n", status);

    my_free(firstAddress);
    status = mem_stats_check(1, 2, 10, 7, 33, 7);
    printf("2 stat after my free: %d\n", status);
    void *ThirdAddress = my_malloc(20);
    my_free(ThirdAddress);
    status = mem_stats_check(1, 2, 10, 7, 33, 7);
    printf("status after freeing the third block: %d\n", status);
    return 0;
}

int my_malloc_1(){
    setUp(20);
    myMallocTest(1);
}

int runTests(void) {
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
    printf("ready for free test!");

    if (my_freeTest()!=0) {
        return -1;
    }

    if (my_freeTest2()!=0) {
        return -1;
    }

    printf("finished all tests!\n");
    print_stats("Trial Run");

    return 0;
    }

int main(int argc, char **argv) {

    printf("Sample main from Prof. Shamash\n\n");

    unsigned int global_mem_size = 1024 * 1024;

    unsigned char *global_memory = malloc(global_mem_size);


    mem_init(global_memory, global_mem_size);

    print_stats("init");


    unsigned char *ptr_array[10];

    unsigned int sizes[] = {50, 20, 20, 20, 50, 0};


    for (int i = 0; sizes[i] != 0; i++) {

        char buf[1024];

        ptr_array[i] = my_malloc(sizes[i]);


        sprintf(buf, "after iteration %d size %d", i, sizes[i]);

        print_stats(buf);

    }


    my_free(ptr_array[1]);
    print_stats("after free #1");


    my_free(ptr_array[3]);
    //printf("Another LINKED LISTS!!!!!!!");
    //printLinkedLists();
    print_stats("after free #3");


    my_free(ptr_array[2]);
    print_stats("after free #2");

printf("Before free LINKED LISTS!!!!!!!");
    printLinkedLists();
    my_free(ptr_array[0]);
    printf("LINKED LISTS!!!!!!!");
    printLinkedLists();
    print_stats("after free #0");


    my_free(ptr_array[4]);
    print_stats("after free #4");

    runTests();
}
