#include <malloc.h>
#include "AllocatePseudoCode.h"

unsigned char* the_memory;

static struct BlockSizesStruct *freeBlocks;
static struct BlockSizesStruct *allocatedBlocks;

// Int addressFirstPositions = Find firstPosition - O(N)
// Iterate through linkedList pointed to by freeBlocks
// Return the yellow A of the first one that is greater than or equal to sizeRequested.

int largest_block_free;
 void mem_init(unsigned char *my_memory, unsigned int my_mem_size) {
     	// ask the os to allocate memory of size my_mem_size. 
        //Set that memory to *my_memory
		the_memory = calloc(1,my_mem_size); //set everything to zero with calloc - for us to use
        my_memory = the_memory;  //return this to the client
        //initializeFreeBlocks();
        addToBlockSizes(my_mem_size, my_memory, 1);   //start free list so my_malloc will work

     //mem_stats_struct *mem_stats_ptr = calloc(1, sizeof(mem_stats_struct));

 }

 //retrieve first layer node that is greater than or equal to size requested - should work for either list
 //return first layer node or NULL if issue of not enough memory
 struct BlockSizesStruct *addressFirstPositions(unsigned int sizeRequested, int isFreeBlocks) {
     struct BlockSizesStruct *oneBlockSizeStruct;
     if (isFreeBlocks == 1) {
        if (freeBlocks == NULL) {
            return NULL; // If is FreeBlocks, then out of memory.
        }
        oneBlockSizeStruct = freeBlocks;
    } else {
        if (allocatedBlocks == NULL) {
            return NULL; // Otherwise, the memory has already been freed.
        }
        oneBlockSizeStruct = allocatedBlocks;
    }
     while (oneBlockSizeStruct->nextNode != NULL) {
         if (oneBlockSizeStruct->blockSize >= sizeRequested) {
             return oneBlockSizeStruct;
         }
         oneBlockSizeStruct = oneBlockSizeStruct->nextNode;
     }
     if (oneBlockSizeStruct->blockSize >= sizeRequested) { // edge case for only one node or last node.
         return oneBlockSizeStruct;
     }
     return NULL; // not enough memory. Usually throw an OutOfMemory exception.
 }

//putting everything together
//can change to char*
void *my_malloc(unsigned size) {
    struct BlockSizesStruct *firstLayerNode = addressFirstPositions(size, 1);
    unsigned char *memory_address = firstLayerNode->addressFP->addressMM;
    addToBlockSizes(size, memory_address, 0); //return blue A
    unsigned int sizeUnused = firstLayerNode->blockSize - size;
    struct MemoryPositionsStruct *tempStruct = firstLayerNode->addressFP->nextNode;
    if (firstLayerNode->addressFP == NULL) {
        removeFree(firstLayerNode, firstLayerNode->addressFP); //cleanup
    }
    // add to the free list however much of the space is not being used - only allocate how much was requested,
    // but the amount requested was probably less than total amount free.
    addToBlockSizes(sizeUnused, memory_address + size, 1);

    //return the address to the client
    return memory_address;

    //pointer to memory location to beg of what was allocated

 }

//first layer node clean up - reset pointers of next and prev nodes and set cur node to null
void removeNodeB(struct BlockSizesStruct *currentNode){
if (currentNode->prevNode != NULL) {
    currentNode->prevNode->nextNode = currentNode->nextNode;
}

if (currentNode->nextNode != NULL){
    currentNode->nextNode->prevNode = currentNode->prevNode;
}
free(currentNode);
int num_blocks_used = num_blocks_used - 1;    //update stats
int num_blocks_free = num_blocks_free + 1;
}

//second layer node clean up
void removeNodeFP(struct MemoryPositionsStruct *currentNode){
    currentNode->prevNode->nextNode = currentNode->nextNode;
    currentNode->nextNode->prevNode = currentNode->prevNode;
    free(currentNode);
//note: don't update stats here because this is just internal things, not part of the actual free blocks list
}

//does more cleanup, esp across the dif layers
void removeFree(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode) { //when allocate memory, it’s not free anymore
// Set the address in the upper layer to not point to firstPosition anymore
//Set the address in the upper layer to point to firstPositions.nextNode,
// if it’s null, remove firstLayer node
//yellow A = currentSecondLayer node
//do .addressFP to get blue A
    currentSecondLayerNode = currentSecondLayerNode->nextNode;
    if (currentSecondLayerNode == NULL) { //if there are no more nodes on the lower leveled linked list, remove the upperlevel node
        removeNodeB(currentFirstLayerNode);
    }
    removeNodeFP(currentSecondLayerNode);
}


void addToBlockSizes(unsigned int sizeRequested, unsigned char *addressInMyMemory, int isFreeBlocks) {
//figure out if free vs allocated list working with now
    struct BlockSizesStruct *firstLayerCurrentNode = isFreeBlocks == 1 ? freeBlocks : allocatedBlocks;

    // find the first layer (BlockSizesStruct) struct that is closest to (but not less than) the sizeRequested
    while (firstLayerCurrentNode != NULL){
        if (firstLayerCurrentNode->blockSize >= sizeRequested){    //works because keeping the list in ascending order
            break;
        }
        else {
            firstLayerCurrentNode = firstLayerCurrentNode->nextNode;
        }
    }

    // If the blockSize is not equal to the sizeRequested, then create a new BlockSizesStruct with blockSize of
    // sizeRequested and set pointers accordingly.
    // -> is the equivalent to *pointer with a dot for retrieving a field. It dereferences the pointer and gets the field
    //   firstLayerCurrentNode->blockSize == (*firstLayerCurrentNode).blockSize
    if (firstLayerCurrentNode == NULL || firstLayerCurrentNode->blockSize != sizeRequested) {
        // create BlockSizesStruct struct for that size
        struct BlockSizesStruct *newAllocatedData = calloc(1, sizeof(struct BlockSizesStruct));
        newAllocatedData->blockSize = sizeRequested;
        newAllocatedData->nextNode = firstLayerCurrentNode;
        newAllocatedData->addressFP = NULL;

        //insert new node before the current firstLayerCurrentNode because it's smaller
        if (firstLayerCurrentNode != NULL) {
            newAllocatedData->prevNode = firstLayerCurrentNode->prevNode;
            if (firstLayerCurrentNode->prevNode != NULL){
                firstLayerCurrentNode->prevNode->nextNode = newAllocatedData;
            }
            firstLayerCurrentNode->prevNode = newAllocatedData;
        }
        firstLayerCurrentNode = newAllocatedData;
       
    }

   if (isFreeBlocks == 1) {
       if (freeBlocks == NULL) {
           freeBlocks = firstLayerCurrentNode;
       }
   } else {
       if (allocatedBlocks == NULL) {
           allocatedBlocks = firstLayerCurrentNode;
       }
   }
   

    // Now that we have the node in the first layer, we need to create a MemoryPositionsStruct to represent the new change
    // Allocate and create new MemoryPositions struct and set addressFP of firstLayerCurrentNode to the new MemoryPositions (blue A)
    struct MemoryPositionsStruct *newMP = calloc(1, sizeof(struct MemoryPositionsStruct));
    newMP->addressMM = addressInMyMemory;
    if (firstLayerCurrentNode->addressFP != NULL) {
        firstLayerCurrentNode->addressFP->prevNode = newMP;
    }
    newMP->nextNode = firstLayerCurrentNode->addressFP;
    newMP->prevNode = NULL;
    firstLayerCurrentNode->addressFP = newMP;

}
int num_blocks_used;
int num_blocks_free;
int smallest_block_free;
int smallest_block_used;  //loop through linkedList - first node should be set to this and last node set to largest blocks...
int largest_block_free;
int largest_block_used;

void mem_get_stats(mem_stats_struct mem_stats_ptr) {
//mem_stats_ptr = struct //????

//loop through whole linked list to get to the end - where the largest block size is
//before start looping, set a variable to be the beg of the linked list.
// The size of this node that is the variable will be what you set smallest_block_free to be.
// Then loop through the linked list until you get to the end. Set that last node's size to be the largest_block_free.
// increment count of free blocks while looping through the list

//(unsigned int sizeRequested, unsigned char *addressInMyMemory, int isFreeBlocks) {
//figure out if free vs allocated list working with now
        struct BlockSizesStruct *firstLayerCurrentNode = isFreeBlocks; //how do you tell it to loop through the free list
    smallest_block_free = firstLayerCurrentNode->blockSize; //since list is in order, the first node will be the smallest
        //loop through the linked list until the end, and set the last node's size to be largest_block_free
        // keep track of how many free blocks there are all together
         while (firstLayerCurrentNode != NULL){
            num_blocks_free = num_blocks_free + 1;
             largest_block_free = firstLayerCurrentNode->blockSize; //if wait until after the loop it would be null
             firstLayerCurrentNode = firstLayerCurrentNode->nextNode;
            }

        //do the same exact thing for the allocated list, just switch variable names appropriately

 }

//mem_stats_ptr.largest_block_free = largest_block;

    //}
