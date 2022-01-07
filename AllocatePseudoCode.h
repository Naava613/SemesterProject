//
// Created by admin on 1/2/2022.
//

#ifndef NAAVA_S_FINAL_ALLOCATEPSEUDOCODE_H
#define NAAVA_S_FINAL_ALLOCATEPSEUDOCODE_H

//make a linked list to hold addresses that have blockSize free spaces
struct MemoryPositionsStruct
{
    unsigned char *addressMM; //address in my_memory space
    struct MemoryPositionsStruct *nextNode;
    struct MemoryPositionsStruct *prevNode;
};

//make a linked list to hold the addresses of places with free/allocated space
struct BlockSizesStruct
{
    unsigned int blockSize;
    struct MemoryPositionsStruct *addressFP; //address of MemoryPositionsStruct
    struct BlockSizesStruct *nextNode;
    struct BlockSizesStruct *prevNode;
};


void mem_init(unsigned char *my_memory, unsigned int my_mem_size);
void *my_malloc(unsigned size);
void my_free(void *mem_pointer);

void addToBlockSizes(unsigned int sizeRequested, unsigned char* addressInMyMemory, int isFreeBlocks);

//Function to create and set block lists from the beginning
struct BlockSizesStruct *addressFirstPositions(unsigned int sizeRequested, int isFreeBlocks);
void removeNodeB(struct BlockSizesStruct *currentNode);
void removeNodeFP(struct MemoryPositionsStruct *currentNode);
void removeFree(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode);

//stats of the code
typedef struct {
    int num_blocks_used;
    int num_blocks_free;
    int smallest_block_free;
    int smallest_block_used;  //loop through linkedList - first node should be set to this and last node set to largest blocks...
    int largest_block_free;
    int largest_block_used;
} mem_stats_struct, *mem_stats_ptr;
void mem_get_stats(mem_stats_ptr mem_stats_ptr);

void printLinkedLists();
#endif //NAAVA_S_FINAL_ALLOCATEPSEUDOCODE_H
