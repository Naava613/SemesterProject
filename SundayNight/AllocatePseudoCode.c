#include <malloc.h>
#include "AllocatePseudoCode.h"

unsigned char* the_memory;

static struct BlockSizesStruct *freeBlocks;
static struct BlockSizesStruct *allocatedBlocks;

// Int addressFirstPositions = Find firstPosition - O(N)
// Iterate through linkedList pointed to by freeBlocks
// Return the yellow A of the first one that is greater than or equal to sizeRequested.

//how do you check for the last node?

 void mem_init(unsigned char *my_memory, unsigned int my_mem_size) {
     	// ask the os to allocate memory of size my_mem_size. 
        //Set that memory to *my_memory
		the_memory = malloc(my_mem_size);
        my_memory = the_memory;
        //initializeFreeBlocks();
        addToBlockSizes(my_mem_size, *my_memory, 1);
 }

 struct MemoryPositionsStruct *addressFirstPositions(unsigned int sizeRequested, int isFreeBlocks) {
     struct BlockSizesStruct oneBlockSizeStruct;
     if (isFreeBlocks == 1) {
        if (freeBlocks == NULL) {
            return NULL; // If is FreeBlocks, then out of memory.
        }
        oneBlockSizeStruct = *freeBlocks;
    } else {
        if (allocatedBlocks == NULL) {
            return NULL; // Otherwise, the memory has already been freed.
        }
        oneBlockSizeStruct = *allocatedBlocks;
    }
     while (oneBlockSizeStruct.nextNode != NULL) {
         if (oneBlockSizeStruct.blockSize >= sizeRequested) {
             return oneBlockSizeStruct.addressFP;
         }
         oneBlockSizeStruct = *oneBlockSizeStruct.nextNode;
     }
     if (oneBlockSizeStruct.blockSize >= sizeRequested) { // edge case for only one node or last node.
         return oneBlockSizeStruct.addressFP;
     }
     return NULL; // not enough memory. Usually throw an OutOfMemory exception.
 }
//    void *my_malloc(unsigned size) {
//        if(freeBlocksList == NULL) {
//    ????
//    }
//    //call to malloc to get sizeRequested amount of memory
//    my_memory = malloc(size)
//
//    // allocate requested memory: check where to put it into BlockSizesStruct
//    addToBlockSizes(size)
//    // and then put into first positions - adding it to each linkedList respectively.
//
//    // update free blocks linked lists properly
//
//
//    }
//}
//
//void removeNode(addressFP){
//
//currentNode.prevNode.nextNode = currentNode.nextNode;
//currentNode.nextNode.prevNode - currentNode.prevNode;
//currentNode = NULL;
//}
//
//void removeFree(addressFP){ //when allocate memory, it’s not free anymore
//// Set the address in the upper layer to not point to firstPosition anymore
//if (firstPositions.nextNode != NULL){
//    blockSizes.addressFP = firstPosition.nextNode
//}
//removeNode(addressFP);
//
//// If no more MemoryPositionsStruct (for that block size), then remove BlockSizesStruct – call removeNode()
//if (addressFP.BlockSizesStruct == NULL){
//    removeNode(addressFP);
//}
//void unless return pointer to new memory
unsigned int addToBlockSizes(unsigned int sizeRequested, unsigned int addressInMyMemory, int isFreeBlocks) {
// Iterate through linkedList pointed to by allocatedBlocks until find first node that is greater than or equal to sizeRequested
//    if (isFreeBlocks == NULL) {
//        // create BlockSizesStruct struct
//        struct BlockSizesStruct *newBlockSizes = malloc(sizeof(struct BlockSizesStruct));
//        // set the block_size to sizeRequested
//        newBlockSizes->blockSize = sizeRequested;
//        isFreeBlocks = newBlockSizes;
//    }
    struct BlockSizesStruct *firstLayerCurrentNode = isFreeBlocks == 1 ? freeBlocks : allocatedBlocks;

    // find the first layer (BlockSizesStruct) struct that is closest to (but not less than) the sizeRequested
    while (firstLayerCurrentNode != NULL){
        if (firstLayerCurrentNode->blockSize >= sizeRequested){
            break;
        }
        else {
            firstLayerCurrentNode = firstLayerCurrentNode->nextNode;
        }
    }

    // If the blockSize is not equal to the sizeRequested, then create a new BlockSizesStruct with blockSize of sizeRequested and set pointers accordingly.
    // The reason for the pointers is to
    // -> is the equivalent to *pointer with a dot for retrieving a field. It dereferences the pointer and gets the field
    //   firstLayerCurrentNode->blockSize == (*firstLayerCurrentNode).blockSize
    if (firstLayerCurrentNode == NULL || firstLayerCurrentNode->blockSize != sizeRequested) {
        // create BlockSizesStruct struct
        struct BlockSizesStruct *newAllocatedData = malloc(sizeof(struct BlockSizesStruct));
        newAllocatedData->blockSize = sizeRequested;
        newAllocatedData->nextNode = firstLayerCurrentNode;
        newAllocatedData->addressFP = NULL;
        newAllocatedData->prevNode == NULL;
        if (firstLayerCurrentNode != NULL) {
            newAllocatedData->prevNode = firstLayerCurrentNode->prevNode;
            firstLayerCurrentNode->prevNode->nextNode = newAllocatedData;
            firstLayerCurrentNode->prevNode = newAllocatedData;
        }
        firstLayerCurrentNode = newAllocatedData;
    }
   // if (isFreeBlocks == NULL) {
   if (isFreeBlocks == 1) {
       if (freeBlocks == NULL) {
           freeBlocks = firstLayerCurrentNode;
       }
   } else {
       if (allocatedBlocks == NULL) {
           allocatedBlocks = firstLayerCurrentNode;
       }
   }
   // }

    // Now that we have the node in the first layer, we need to create a MemoryPositionsStruct to represent the new change
    // Allocate and create new MemoryPositions struct and set addressFP of firstLayerCurrentNode to the new MemoryPositions (blue A)
    struct MemoryPositionsStruct *newMP = malloc(sizeof(struct MemoryPositionsStruct));
    newMP->addressMM = addressInMyMemory;
    if (firstLayerCurrentNode->addressFP != NULL) {
        firstLayerCurrentNode->addressFP->prevNode = newMP;
    }
    newMP->nextNode = firstLayerCurrentNode->addressFP;
    newMP->prevNode = NULL;
    firstLayerCurrentNode->addressFP = newMP;

    return addressInMyMemory; //(blue A)
}

//void mem_get_stats(mem_stats_ptr mem_stats_ptr);
//
//void mem_get_stats(mem_stats_ptr mem_stats_ptr){
//mem_stats_ptr = struct //????
//typedef struct  {
//  int num_blocks_used;
//  int num_blocks_free;
//  int smallest_block_free;
//  int smallest_block_used;
//  int largest_block_free;
//  int largest_block_used;
//} mem_stats_struct, *mem_stats_ptr;
//
