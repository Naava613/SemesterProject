#include <malloc.h>
#include "AllocatePseudoCode.h"

unsigned char *the_memory;

static struct BlockSizesStruct *freeBlocks;
static struct BlockSizesStruct *allocatedBlocks;

void coalesce(unsigned char *mem_pointer, unsigned int block_size_to_free);

// Int addressFirstPositions = Find firstPosition - O(N)
// Iterate through linkedList pointed to by freeBlocks
// Return the yellow A of the first one that is greater than or equal to sizeRequested.

void mem_init(unsigned char *my_memory, unsigned int my_mem_size) {
    // ask the os to allocate memory of size my_mem_size.
    //Set that memory to *my_memory
    freeBlocks = NULL;
    allocatedBlocks = NULL;
    the_memory = calloc(1, my_mem_size); //set everything to zero with calloc - for us to use
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
    if (firstLayerNode == NULL) {
        return "Not enough memory";
    }
    unsigned char *memory_address = firstLayerNode->addressFP->addressMM;
    addToBlockSizes(size, memory_address, 0); //return blue A
    unsigned int sizeUnused = firstLayerNode->blockSize - size;
    struct MemoryPositionsStruct *tempStruct = firstLayerNode->addressFP->nextNode;
    removeFree(firstLayerNode, firstLayerNode->addressFP); //cleanup
    // add to the free list however much of the space is not being used - only allocate how much was requested,
    // but the amount requested was probably less than total amount free.
    addToBlockSizes(sizeUnused, memory_address + size, 1);

    //return the address to the client
    return memory_address;

    //pointer to memory location to beg of what was allocated

}

//first layer node clean up - reset pointers of next and prev nodes and set cur node to null
void removeNodeB(struct BlockSizesStruct *currentNode) {
    if (currentNode->prevNode != NULL) {
        currentNode->prevNode->nextNode = currentNode->nextNode;
    }

    if (currentNode->nextNode != NULL) {
        currentNode->nextNode->prevNode = currentNode->prevNode;
    }
    if (freeBlocks == currentNode) {
        freeBlocks = currentNode->nextNode;
    } else if (allocatedBlocks == currentNode) {
        allocatedBlocks = currentNode->nextNode;
    }
    currentNode->nextNode = NULL;
    currentNode->prevNode = NULL;
    free(currentNode);
}

//second layer node clean up
void removeNodeFP(struct MemoryPositionsStruct *currentNode) {
    if (currentNode != NULL) {
        if (currentNode->prevNode != NULL) {
            currentNode->prevNode->nextNode = currentNode->nextNode;
        }
        if (currentNode->nextNode != NULL) {
            currentNode->nextNode->prevNode = currentNode->prevNode;
        }
        free(currentNode);
    }
//note: don't update stats here because this is just internal things, not part of the actual free blocks list
}

//does more cleanup, esp across the dif layers
void removeFree(struct BlockSizesStruct *currentFirstLayerNode,
                struct MemoryPositionsStruct *currentSecondLayerNode) { //when allocate memory, it’s not free anymore
// Set the address in the upper layer to not point to firstPosition anymore
//Set the address in the upper layer to point to firstPositions.nextNode,
// if it’s null, remove firstLayer node
//yellow A = currentSecondLayer node
//do .addressFP to get blue A
    currentSecondLayerNode = currentSecondLayerNode->nextNode;
    if (currentSecondLayerNode ==
        NULL) { //if there are no more nodes on the lower leveled linked list, remove the upperlevel node
        removeNodeB(currentFirstLayerNode);
    }
    removeNodeFP(currentSecondLayerNode);
}


void addToBlockSizes(unsigned int sizeRequested, unsigned char *addressInMyMemory, int isFreeBlocks) {
//figure out if free vs allocated list working with now
    struct BlockSizesStruct *firstLayerCurrentNode = isFreeBlocks == 1 ? freeBlocks : allocatedBlocks;
    struct BlockSizesStruct *previousNode = NULL;

    // find the first layer (BlockSizesStruct) struct that is closest to (but not less than) the sizeRequested
    while (firstLayerCurrentNode != NULL) {
        if (firstLayerCurrentNode->blockSize >= sizeRequested) {    //works because keeping the list in ascending order
            break;
        } else {
            previousNode = firstLayerCurrentNode;
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
            if (firstLayerCurrentNode->prevNode != NULL) {
                firstLayerCurrentNode->prevNode->nextNode = newAllocatedData;
            }
            firstLayerCurrentNode->prevNode = newAllocatedData;
            if (isFreeBlocks == 1) {
                if (freeBlocks == firstLayerCurrentNode) {
                    freeBlocks = newAllocatedData;
                }
            } else {
                if (allocatedBlocks == firstLayerCurrentNode) {
                    allocatedBlocks = newAllocatedData;
                }
            }
        } else {
            // append node to linked list
            if (previousNode != NULL) {
                previousNode->nextNode = newAllocatedData;
                newAllocatedData->prevNode = previousNode;
            }
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
int smallest_block_used;  //loop through linkedList - first node should be set to this and last node set to largest blocks...
int largest_block_free;
int largest_block_used;

void mem_get_stats(mem_stats_ptr mem_stats_ptr) {

    // get stats for the free list
    mem_stats_ptr->smallest_block_free = 0;
    mem_stats_ptr->largest_block_free = 0;
    mem_stats_ptr->num_blocks_free = 0;

    if (freeBlocks != NULL) {
        // Since list is in order, the first node will be the smallest
        mem_stats_ptr->smallest_block_free = freeBlocks->blockSize;

        //loop through the linked list until the end, and set the last node's size to be largest_block_free
        // keep track of how many free blocks there are all together
        struct BlockSizesStruct *firstLayerCurrentNode = freeBlocks;
        while (firstLayerCurrentNode != NULL) {
            mem_stats_ptr->num_blocks_free = mem_stats_ptr->num_blocks_free + 1;
            mem_stats_ptr->largest_block_free = firstLayerCurrentNode->blockSize; //if wait until after the loop it would be null
            struct MemoryPositionsStruct *secondLayerCurrentNode = firstLayerCurrentNode->addressFP;
            while (secondLayerCurrentNode != NULL) {
                mem_stats_ptr->num_blocks_used = mem_stats_ptr->num_blocks_used + 1;
                secondLayerCurrentNode = secondLayerCurrentNode->nextNode;
            }
            firstLayerCurrentNode = firstLayerCurrentNode->nextNode;
        }
    }

    // get stats for the allocated list
    mem_stats_ptr->smallest_block_used = 0;
    mem_stats_ptr->largest_block_used = 0;
    mem_stats_ptr->num_blocks_used = 0;
    if (allocatedBlocks != NULL) {
        // Since list is in order, the first node will be the smallest
        mem_stats_ptr->smallest_block_used = allocatedBlocks->blockSize;

        //loop through the linked list until the end, and set the last node's size to be largest_block_free
        // keep track of how many free blocks there are all together
        struct BlockSizesStruct *firstLayerCurrentNode = allocatedBlocks;
        while (firstLayerCurrentNode != NULL) {
            mem_stats_ptr->largest_block_used = firstLayerCurrentNode->blockSize; //if wait until after the loop it would be null
            struct MemoryPositionsStruct *secondLayerCurrentNode = firstLayerCurrentNode->addressFP;
            while (secondLayerCurrentNode != NULL) {
                mem_stats_ptr->num_blocks_used = mem_stats_ptr->num_blocks_used + 1;
                secondLayerCurrentNode = secondLayerCurrentNode->nextNode;
            }
            firstLayerCurrentNode = firstLayerCurrentNode->nextNode;
        }
    }
}

// returns the memory to the pool passed to mem_init().
void my_free(void *mem_pointer) {
    //mem_pointer is pointer to the beg of the memory position of the block that's being freed - ie. the blueA
    //depth-first search
    struct BlockSizesStruct *allocatedFirstLayerNode = allocatedBlocks;
    struct MemoryPositionsStruct *allocatedSecondLayerNode;
    unsigned int block_size_to_free = 0;
    int foundIt = 0;
    while (allocatedFirstLayerNode != NULL && !foundIt) {
        allocatedSecondLayerNode = allocatedFirstLayerNode->addressFP;
        while (allocatedSecondLayerNode != NULL && !foundIt) {
            if (allocatedSecondLayerNode->addressMM == mem_pointer) {
                //check if there are other nodes on the second layer to know if should set currentFirstLayer node to null
                block_size_to_free = allocatedFirstLayerNode->blockSize;
                if (allocatedSecondLayerNode->nextNode == NULL && allocatedSecondLayerNode->prevNode == NULL) {
                    removeNodeFP(allocatedSecondLayerNode);
                    removeNodeB(allocatedFirstLayerNode);
                } else {
                    removeNodeFP(allocatedSecondLayerNode);
                }
                foundIt = 1;
            }
            allocatedSecondLayerNode = allocatedSecondLayerNode->nextNode;
        }
        allocatedFirstLayerNode = allocatedFirstLayerNode->nextNode;
    }
    coalesce(mem_pointer, block_size_to_free);

}

//look through my_memory array and put together all the free spaces
int cleanUpForCoalesce(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode);
int cleanUpForCoalesce(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode) {
    if (currentFirstLayerNode->addressFP == currentSecondLayerNode) {   //if the firstLayer block is pointing to the second layer block
        if (currentSecondLayerNode->nextNode == NULL) {                    // and secondLayerBlock is the only block for that size
            removeNodeFP(currentSecondLayerNode);                  // then remove corresponding blocks in both layers
            removeNodeB(currentFirstLayerNode);
        } else if (currentSecondLayerNode->nextNode != NULL) {
            //set firstLayerNode to point to second layer node.next and then remove node from second layer
            currentFirstLayerNode->addressFP = currentSecondLayerNode->nextNode;
            removeNodeFP(currentSecondLayerNode);
        }
    }
    else {
        removeNodeFP(currentSecondLayerNode);
    }
}
void coalesce(unsigned char *mem_pointer, unsigned int block_size_to_free) {
    //start from first position (addressMM):
    //long long startingAddress = &mem_pointer - &the_memory;
    struct BlockSizesStruct *currentFirstLayerNode = freeBlocks;
    struct MemoryPositionsStruct *currentSecondLayerNode;
    unsigned int newSize = block_size_to_free; //size of new block to be added to the list is at least size of block being freed

    int numberAdjacentBlocksFree = 0;
    while (currentFirstLayerNode != NULL && numberAdjacentBlocksFree < 2) {
        struct BlockSizesStruct *nextFreeFirstLayerNode = currentFirstLayerNode->nextNode;
        currentSecondLayerNode = currentFirstLayerNode->addressFP;
        while (currentSecondLayerNode != NULL) {
            // before block
            struct MemoryPositionsStruct *nextFreeSecondLayerNode = currentSecondLayerNode->nextNode;
            if (mem_pointer == currentSecondLayerNode->addressMM + currentFirstLayerNode->blockSize) {
                //add blockSize to size of the thing you just freed
                newSize = newSize + currentFirstLayerNode->blockSize;
                cleanUpForCoalesce(currentFirstLayerNode, currentSecondLayerNode);
                numberAdjacentBlocksFree++;
            }
                // check after freed block
            else if (mem_pointer + block_size_to_free == currentSecondLayerNode->addressMM) {
                newSize = newSize + currentFirstLayerNode->blockSize;
                cleanUpForCoalesce(currentFirstLayerNode, currentSecondLayerNode);
                numberAdjacentBlocksFree++;
            }
            currentSecondLayerNode = nextFreeSecondLayerNode;
        }
        currentFirstLayerNode = nextFreeFirstLayerNode;

    }
        addToBlockSizes(newSize, mem_pointer, 1);

//look for the space in the array of allocated sizes, set it to null - removeNodeFP (reset pointers of next and prev nodes)

//if no more second layer nodes, then removeNodeB

//add it back to free sizes (maybe can use addToBlockSizes function, set it to one)

//call function to coalesce the free spaces together


}
