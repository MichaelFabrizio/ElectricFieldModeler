#include "MonotonicAllocator.h"
#include <cstddef>
#include <iostream>

MonotonicAllocator::MonotonicAllocator() : offsetPointer_(0), blockSize_(0)
{
	//Arbitrary size for now
	memoryAddress_ = MallocInternal(50000000);
}

void* MonotonicAllocator::AllocateMemory(std::size_t size)
{
	if ((offsetPointer_ + size) > blockSize_)
	{
		const char* stackError = "Heap storage insufficient";
		std::cout << stackError << '\n';

		// TODO: Allow allocation of a new block of memory, build logic for size requirements
		exit(-1);
		return nullptr;
	}

	char* memPtr = (char*)memoryAddress_ + offsetPointer_;
	offsetPointer_ += size;
	return memPtr;
}

std::size_t MonotonicAllocator::GetAllocationSize() const
{
	return offsetPointer_;
}

void* MonotonicAllocator::MallocInternal(std::size_t size)
{
	auto memBlock = malloc(size);

	//TODO: Add functionality so that a failed malloc doesn't ALWAYS "crash" the program
	if (!memBlock)
	{
		const char* stackError = "Memory allocation failed";
		std::cout << stackError << '\n';

		exit(-1);
		return nullptr;
	}

	blockSize_ += size;

	return memBlock;
}