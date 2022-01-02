//Allocate(sizeRequested):
// Int addressFirstPositions = Find firstPosition - O(N)
// Iterate through linkedList pointed to by freeBlocks
// Return the yellow A of the first one that is greater than or equal to sizeRequested.
// Void remove_free(addressFirstPositions) – when allocate memory, it’s not free anymore
// Set the address in the upper layer to not point to firstPosition anymore
// removeNode(addressFirstPositions)
// If no more firstPositions (for that block size), then remove blockSizes – call removeNode()
// Void addAllocated() – void unless return pointer to new memory
// Iterate through linkedList pointed to by allocatedBlocks until find first node that is greater than or equal to sizeRequested
// firstLayerCurrentNode = first node >= sizeRequested
// If blockSizes.block_size != sizeRequested
// allocate new blockSizes struct
// set firstLayerCurrentNode.previousNode.nextNode to blockSizes
// set blockSizes.previousNode to firstLayerCurrentNode.previousNode. 
// Set firstLayerCurrentNode.previousNode to new blockSizes. 
// Set blockSizes.nextNode to firstLayerCurrentNode 
// firstLayerCurrentNode = blockSizes
// Allocated new FirstPositions struct and set my_memory_address to *addressFirstPositions.address (blue A)
// Set positions
// firstPositions.nextNode = firstLayerCurrentNode.address (yellow A)
// firstPositions.nextNode.previousNode = firstPositions
// firstLayerCurrentNode.address = firstPositions
// Return *addressFirstPositions.address (blue A)
