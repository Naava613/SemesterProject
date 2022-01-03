//
// Created by admin on 1/2/2022.
//

#ifndef NAAVA_S_FINAL_ALLOCATEPSEUDOCODE_H
#define NAAVA_S_FINAL_ALLOCATEPSEUDOCODE_H

//make a linked list to hold addresses that have blockSize free spaces
struct MemoryPositionsStruct
{
    unsigned int addressMM; //address in my_memory space
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
unsigned int addToBlockSizes(unsigned int sizeRequested, unsigned int addressInMyMemory, int isFreeBlocks);

//Function to create and set block lists from the beginning
struct MemoryPositionsStruct *addressFirstPositions(unsigned int sizeRequested, int isFreeBlocks);


#endif //NAAVA_S_FINAL_ALLOCATEPSEUDOCODE_H
