#include "../h/memoryAllocator.hpp"

MemoryAllocator::FreeMem* MemoryAllocator::freeHead = nullptr;

void MemoryAllocator::memInit() {
    freeHead = (FreeMem*)HEAP_START_ADDR;
    *freeHead = {
            nullptr,
            ((size_t)(HEAP_END_ADDR) - (size_t)(HEAP_START_ADDR) - sizeof(FreeMem)) / MEM_BLOCK_SIZE
    };
}

void* MemoryAllocator::memAlloc(size_t size) {
    size_t neededSize = size + sizeof(FreeMem);
    size_t sizeInBlocks = (neededSize / MEM_BLOCK_SIZE) + (neededSize % MEM_BLOCK_SIZE == 0 ? 0 : 1);
    void* allocatedAddr = nullptr;

    if (!freeHead) return nullptr;

    FreeMem* curr = freeHead;
    FreeMem* prev = nullptr;

    // first-fit
    while (curr) {
        if (curr->size >= sizeInBlocks) {
            // found a big enough fragment

            // remove it from the free fragments linked list
            if (!prev) { // it's the first element in the linked list
                if (curr->size - sizeInBlocks > 0) {
                    // shrink the free fragment
                    freeHead = curr + sizeInBlocks * MEM_BLOCK_SIZE;
                    freeHead->size = curr->size - sizeInBlocks;
                    freeHead->next = curr->next;

                    curr->size = sizeInBlocks; // update the allocated fragment size
                } else {
                    freeHead = freeHead->next;
                }

                allocatedAddr = curr;
            } else if (!curr->next) { // it's the last element in the linked list
                if (curr->size - sizeInBlocks > 0) {
                    // shrink the fragment
                    FreeMem* newFreeFragmentAddr = curr + sizeInBlocks * MEM_BLOCK_SIZE;
                    prev->next = newFreeFragmentAddr;
                    newFreeFragmentAddr->size = curr->size - sizeInBlocks;
                    newFreeFragmentAddr->next = curr->next;

                    curr->size = sizeInBlocks; // update the allocated fragment size
                } else {
                    prev->next = nullptr;
                }

                allocatedAddr = curr;
            } else {
                if (curr->size - sizeInBlocks > 0) {
                    // shrink the fragment
                    FreeMem* newFreeFragmentAddr = curr + sizeInBlocks * MEM_BLOCK_SIZE;
                    prev->next = newFreeFragmentAddr;
                    newFreeFragmentAddr->size = curr->size - sizeInBlocks;
                    newFreeFragmentAddr->next = curr->next;

                    curr->size = sizeInBlocks; // update the allocated fragment size
                } else {
                    prev->next = curr->next;
                }

                allocatedAddr = curr;
            }

            break;
        }

        prev = curr;
        curr = curr->next;
    }

    return (allocatedAddr ? ((FreeMem*)allocatedAddr + 1) : nullptr);
}

int MemoryAllocator::memFree(void *startAddr) {
    if (!startAddr) return -1;

    FreeMem* freedBlock = (FreeMem*)startAddr - 1;
    FreeMem* curr = freeHead;
    FreeMem* prev = nullptr;

    if (startAddr < HEAP_START_ADDR || startAddr >= HEAP_END_ADDR) return -1;

    // the whole address space was allocated before
    if (!curr) {
        freeHead = freedBlock;
        freedBlock->next = nullptr;
        return 0;
    }

    if (freedBlock < curr) {
        freedBlock->next = freeHead;
        freeHead = freedBlock;
        tryToMerge(freeHead);
        return 0;
    }

    // insert into linked list in correct order
    while (freedBlock > curr && curr != nullptr) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        prev->next = freedBlock;
        freedBlock->next = nullptr;
        tryToMerge(prev);
        return 0;
    }

    prev->next = freedBlock;
    freedBlock->next = curr;
    tryToMerge(freedBlock);
    tryToMerge(prev);

    return 0;
}

// this implementation was taken from "OS1 KLK2 SEPT2015 2"
void MemoryAllocator::tryToMerge(FreeMem* fragment) {
    if (!fragment) return;
    if (fragment->next && (char*)fragment + fragment->size * MEM_BLOCK_SIZE == (char*)fragment->next) {
            // remove the fragment->next
            fragment->size += fragment->next->size;
            fragment->next = fragment->next->next;
    } else return;
}