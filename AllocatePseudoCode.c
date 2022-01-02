//Allocate(sizeRequested):
int freeBlocks;//??
int allocatedBlocks; //??

char the_memory[];

//make a linked list to hold addresses that have blockSize free spaces
typedef struct firstPositions
{
   int addressMM; //address in my_memory space
   int nextNode;
   int prevNode;  
};

//make a linked list to hold the addresses of places with free/allocated space
struct blockSizes 
{
    int blockSize;
    firstPositions addressFP; //address of firstPositions
    int nextNode;
    int prevNode; 
};


// Int addressFirstPositions = Find firstPosition - O(N)
// Iterate through linkedList pointed to by freeBlocks
// Return the yellow A of the first one that is greater than or equal to sizeRequested.

//how do you check for the last node?
struct blockSizes freeBlocksList;  //create a type of the sturcture blockSizes which will be for free spaces

 void mem_init(unsigned char *my_memory, unsigned int my_mem_size) {
     	// ask the os to allocate memory of size my_mem_size. 
        //Set that memory to *my_memory
		*the_memory = malloc(my_mem_size);
        my_memory = the_memory;
        // add to freeBlocksList
        initializeLists(my_memory, my_mem_size); 
 }

 //Function to create and set block lists from the beginning
 void addFirstPositionsNode(my_memory, my_mem_size);

 void addFirstPositionsNode(my_memory, my_mem_size, blocks = freeBlocksList){
        // first create firstPositions struct

        // see if blockSizes exists?
        // if it does not, then create blockSizes struct
            // set nn  and pn pointers accordingly
        // set the address of blockSizes to 
        
        addressMM.firstPositions = my_memory;
        nextNode.firstPositions = NULL;
        prevNode.FirstPositions = NULL;
        //maybe don't need the .blockSize here?
        //how do you make this function useable for both free and allocated?
        //would you just pass that into the parameters of the function?
        freeBlocks = 
        addressFP.freeBlocksList = firstPositions;
        //Do I need to initialize prev and next nodes here? nextNode.freeBlocksList 
 }
void *my_malloc(unsigned size) {
    if(freeBlocksList == NULL) {

    }
    while (freeBlocksList.nextNode != NULL){
        if (freeBlocksList.addressFP >= sizeRequested){
            return addressFP
        }
    }
}

void removeNode(addressFP){
    
currentNode.prevNode.nextNode = currentNode.nextNode;
currentNode.nextNode.prevNode - currentNode.prevNode;
currentNode = NULL;
}

void removeFree(addressFP){ //when allocate memory, it’s not free anymore
// Set the address in the upper layer to not point to firstPosition anymore
if (firstPositions.nextNode != Null){
    blockSizes.addressFP = firstPosition.nextNode
}
removeNode(addressFP);

// If no more firstPositions (for that block size), then remove blockSizes – call removeNode()
if (addressFP.blockSizes == NULL){
    removeNode(addressFP);
}
void addAllocated();
//void unless return pointer to new memory
void addAllocated(unsigned int requestedSize, unsigned int addressInMyMemory) {
// Iterate through linkedList pointed to by allocatedBlocks until find first node that is greater than or equal to sizeRequested
    if (allocatedBlocks == -1) {
        // create blockSizes struct
        // set the block_size to requestedSize
    }
    firstLayerCurrentNode = *allocatedBlocks;
    // find the first layer (blockSizes) struct that is closest (but not less than the requestedSize)
    while (firstLayerCurrentNode != NULL){
        if (firstLayerCurrentNode.block_size >= sizeRequested){
            break;
        }
        else{
            firstLayerCurrentNode = firstLayerCurrentNode.nextNode;
        }
    
    }
    if (firstLayerCurrentNode.block_size != sizeRequested) {
        // create blockSizes struct
        newAllocatedData = malloc(sizeof(blockSizes));
        newAllocatedData.blockSize = sizeRequested;
        firstLayerCurrentNode.previousNode.nextNode = newAllocatedData
        newAllocatedData.previousNode = firstLayerCurrentNode.previousNode; 
        firstLayerCurrentNode.previousNode = newAllocatedData;
        newAllocatedData.nextNode = firstLayerCurrentNode;
        firstLayerCurrentNode = newAllocatedData;
    }
    
    if (blockSizes.blockSize != sizeRequested){
        // allocate new blockSizes struct
        allocatedBlocks = my_malloc(sizeRequested);
        firstLayerCurrentNode.previousNode.nextNode = blockSizes;
        blockSizes.previousNode = firstLayerCurrentNode.previousNode; 
        firstLayerCurrentNode.previousNode = allocatedBlocks; 
        blockSizes.nextNode = firstLayerCurrentNode; 
        firstLayerCurrentNode = blockSizes;
    }

// Allocate new FirstPositions struct and set my_memory_address to *addressFirstPositions.address (blue A)
    char newFP = malloc(sizeof(firstPositions));
    my_memory_address = *addressFirstPositions.address //(blue A)

// create firstPositions
    firstPositions.nextNode = firstLayerCurrentNode.address; //(yellow A)
    firstPositions.nextNode.previousNode = firstPositions;
    firstLayerCurrentNode.address = firstPositions;

return *addressFirstPositions.address; //(blue A)

void mem_get_stats(mem_stats_ptr mem_stats_ptr);

void mem_get_stats(mem_stats_ptr mem_stats_ptr){
mem_stats_ptr = struct //????
typedef struct  {
  int num_blocks_used;
  int num_blocks_free;
  int smallest_block_free;
  int smallest_block_used;
  int largest_block_free;
  int largest_block_used;
} mem_stats_struct, *mem_stats_ptr;

