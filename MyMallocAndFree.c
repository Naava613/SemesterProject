#include <malloc.h>
#include "AllocatePseudoCode.h"
#include <stdio.h>

// define or initialize needed things
unsigned char *the_memory;

static struct BlockSizesStruct *freeBlocks;
static struct BlockSizesStruct *allocatedBlocks;

void coalesce(unsigned char *mem_pointer, unsigned int block_size_to_free);
int cleanUpForCoalesce(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode);


void mem_init(unsigned char *my_memory, unsigned int my_mem_size) {
    // ask the os to allocate memory of size my_mem_size
    //Set that memory to *my_memory
    freeBlocks = NULL;
    allocatedBlocks = NULL;
    the_memory = calloc(1, my_mem_size); //set everything to zero with calloc - for easier use
    my_memory = the_memory;  //return this to the client
    addToBlockSizes(my_mem_size, my_memory, 1);   //start free list so my_malloc will work

}


//returns first layer node or NULL if issue of not enough memory
struct BlockSizesStruct *addressFirstPositions(unsigned int sizeRequested, int isFreeBlocks) {
    struct BlockSizesStruct *oneBlockSizeStruct;
    //check which list we are looking at and set appropriately
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

    //go through list to find the correct node, if any exist
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

    // add to the free list however much of the space is not being used
    addToBlockSizes(sizeUnused, memory_address + size, 1);

    //return the address to the client
    return memory_address;
}

//first layer nodes clean up - reset pointers of next and prev nodes and set cur node to null
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
}

//does more cleanup, specifically across different layers
// notes on the lingo here: yellow A = currentSecondLayer node and ->addressFP means blue A in the diagram
void removeFree(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode) {

// Set the address in the upper layer to not point to firstPosition anymore, but rather to point to firstPositions.nextNode
    currentSecondLayerNode = currentSecondLayerNode->nextNode;
    // if it’s null, remove firstLayer node
    if (currentSecondLayerNode ==  NULL) {
        removeNodeB(currentFirstLayerNode);
    }
    removeNodeFP(currentSecondLayerNode);
}


void addToBlockSizes(unsigned int sizeRequested, unsigned char *addressInMyMemory, int isFreeBlocks) {
//figure out if working with free vs allocated list now
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

        //reset nodes
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
            // append node to linked list - got to end because no node in list is greater than or equal to sizeReqeusted
            if (previousNode != NULL) {
                previousNode->nextNode = newAllocatedData;
                newAllocatedData->prevNode = previousNode;
            }
        }
        //update to keep looping
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

void mem_get_stats(mem_stats_ptr mem_stats_ptr) {

    // get stats for the free list
    mem_stats_ptr->smallest_block_free = 0;
    mem_stats_ptr->largest_block_free = 0;
    mem_stats_ptr->num_blocks_free = 0;

    if (freeBlocks != NULL) {
        // Since list is in order, the first node will be the smallest
        //remember: freeBlocks is the int that points to first of the free blocks list
        mem_stats_ptr->smallest_block_free = freeBlocks->blockSize;

        //loop through the linked list until the end, and set the last node's size to be largest_block_free
        // keep track of how many free blocks there are all together
        struct BlockSizesStruct *firstLayerCurrentNode = freeBlocks;
        while (firstLayerCurrentNode != NULL) {
            mem_stats_ptr->largest_block_free = firstLayerCurrentNode->blockSize; //if wait until after the loop it would be null
            struct MemoryPositionsStruct *secondLayerCurrentNode = firstLayerCurrentNode->addressFP;
            while (secondLayerCurrentNode != NULL) {
                mem_stats_ptr->num_blocks_free = mem_stats_ptr->num_blocks_free + 1;   //I'm changing this to free from used
                secondLayerCurrentNode = secondLayerCurrentNode->nextNode;
            }
            //update to keep looping
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
            //update to keep looping
            firstLayerCurrentNode = firstLayerCurrentNode->nextNode;
        }
    }
}

// returns the memory to the pool passed to mem_init()
//mem_pointer is pointer to the beg of the memory position of the block that's being freed - ie. the blueA
void my_free(void *mem_pointer) {
    struct BlockSizesStruct *allocatedFirstLayerNode = allocatedBlocks;
    struct MemoryPositionsStruct *allocatedSecondLayerNode;
    unsigned int block_size_to_free = 0;
    int foundIt = 0;

    //depth-first search
    while (allocatedFirstLayerNode != NULL && !foundIt) {
        allocatedSecondLayerNode = allocatedFirstLayerNode->addressFP;
        struct BlockSizesStruct *nextAllocFirstLayerNode = allocatedFirstLayerNode->nextNode;

        while (allocatedSecondLayerNode != NULL && !foundIt) {
            struct MemoryPositionsStruct *nextAllocSecondLayerNode = allocatedSecondLayerNode->nextNode;
            //find the right node to free
            if (allocatedSecondLayerNode->addressMM == mem_pointer) {
                block_size_to_free = allocatedFirstLayerNode->blockSize;
                //check if there are other nodes on the second layer to know if should set currentFirstLayer node to null

                //if no other nodes, just remove that node
                if (allocatedSecondLayerNode->nextNode == NULL && allocatedSecondLayerNode->prevNode == NULL) {
                    removeNodeFP(allocatedSecondLayerNode);
                    removeNodeB(allocatedFirstLayerNode);
                } else {
                    //save secondlayernode so you can free it correctly later
                    struct MemoryPositionsStruct *secondLayerNodeToDelete = allocatedSecondLayerNode;

                    //if it's the first node of second layer
                    if (allocatedSecondLayerNode->nextNode != NULL && allocatedSecondLayerNode->prevNode == NULL) {
                        allocatedFirstLayerNode->addressFP = allocatedSecondLayerNode->nextNode;
                        //reset pointers within the second layer. prev pointer to be null
                        allocatedSecondLayerNode->nextNode->prevNode = NULL;
                        allocatedSecondLayerNode = nextAllocSecondLayerNode;

                    }
                    //if it's the middle node of second layer
                    else if (allocatedSecondLayerNode->nextNode != NULL && allocatedSecondLayerNode->prevNode != NULL){
                        allocatedSecondLayerNode->nextNode->prevNode = allocatedSecondLayerNode->prevNode;
                        allocatedSecondLayerNode->prevNode->nextNode = nextAllocSecondLayerNode;
                        }
                    removeNodeFP(secondLayerNodeToDelete);
                }
                foundIt = 1;   //so that we come out of the loop properly
            }
            //update to keep looping
            allocatedSecondLayerNode = nextAllocSecondLayerNode;
        }
        //update to keep looping
        allocatedFirstLayerNode = nextAllocFirstLayerNode;
    }
    // put together the fragmented pieces
    coalesce(mem_pointer, block_size_to_free);

}

//look through my_memory array and put together all the free spaces
int cleanUpForCoalesce(struct BlockSizesStruct *currentFirstLayerNode, struct MemoryPositionsStruct *currentSecondLayerNode) {
    if (currentFirstLayerNode->addressFP == currentSecondLayerNode) {   //if the firstLayer block is pointing to the second layer block
        if (currentSecondLayerNode->nextNode == NULL) {                    // and secondLayerBlock is the only block for that size
            removeNodeFP(currentSecondLayerNode);                  // then remove corresponding blocks in both layers
            removeNodeB(currentFirstLayerNode);
        }

        else if (currentSecondLayerNode->nextNode != NULL) {
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
    //start from first position (addressMM)
    struct BlockSizesStruct *currentFirstLayerNode = freeBlocks;
    struct MemoryPositionsStruct *currentSecondLayerNode;
    unsigned int newSize = block_size_to_free; //size of new block to be added to the list is at least size of block being freed

    int numberAdjacentBlocksFree = 0;                     //to keep track and help us not keep going in loop too many times
    unsigned char *earliest_mem_address = mem_pointer;   //need the correct mem_pointer for later calculations

    //loop through the linked lists, checking for free blocks next to the one you just freed
    while (currentFirstLayerNode != NULL && numberAdjacentBlocksFree < 2) {
        struct BlockSizesStruct *nextFreeFirstLayerNode = currentFirstLayerNode->nextNode;
        currentSecondLayerNode = currentFirstLayerNode->addressFP;

        //loop through second layer
        while (currentSecondLayerNode != NULL) {

            // check before the block that was just freed
            struct MemoryPositionsStruct *nextFreeSecondLayerNode = currentSecondLayerNode->nextNode;
            if (mem_pointer == currentSecondLayerNode->addressMM + currentFirstLayerNode->blockSize) {
                //add blockSize to size of the thing you just freed
                newSize = newSize + currentFirstLayerNode->blockSize;
                cleanUpForCoalesce(currentFirstLayerNode, currentSecondLayerNode);
                numberAdjacentBlocksFree++;
                earliest_mem_address = currentSecondLayerNode->addressMM;
            }
                // check after freed block
            else if (mem_pointer + block_size_to_free == currentSecondLayerNode->addressMM) {
                newSize = newSize + currentFirstLayerNode->blockSize;
                cleanUpForCoalesce(currentFirstLayerNode, currentSecondLayerNode);
                numberAdjacentBlocksFree++;
            }
            //update to keep looping
            currentSecondLayerNode = nextFreeSecondLayerNode;
        }
        //update so it will keep looping
        currentFirstLayerNode = nextFreeFirstLayerNode;

    }
        //add the new info to the free list (if isFreeBlocks == 1, it means yes, it's free)
        addToBlockSizes(newSize, earliest_mem_address, 1);

}


// Utility function used during debugging to get a visual of the linked lists
// Not really needed for this whole malloc/free program.
void printLinkedLists();
void printLinkedLists() {
    struct BlockSizesStruct *allocatedFirstLayerNode = allocatedBlocks;
    struct MemoryPositionsStruct *allocatedSecondLayerNode;

    while (allocatedFirstLayerNode != NULL) {
        printf("F: %d", allocatedFirstLayerNode->blockSize);
        allocatedSecondLayerNode = allocatedFirstLayerNode->addressFP;
        struct BlockSizesStruct *nextAllocFirstLayerNode = allocatedFirstLayerNode->nextNode;

        while (allocatedSecondLayerNode != NULL) {
            //struct MemoryPositionsStruct *nextAllocSecondLayerNode = allocatedSecondLayerNode->nextNode;
            printf("->S: %d  ", allocatedSecondLayerNode);
            allocatedSecondLayerNode = allocatedSecondLayerNode->nextNode;
        }
        allocatedFirstLayerNode = nextAllocFirstLayerNode;
    }
}