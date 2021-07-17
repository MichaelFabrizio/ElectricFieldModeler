#pragma once
#include <cstddef>

class MonotonicAllocator
{
public:
	explicit MonotonicAllocator();
	MonotonicAllocator(const MonotonicAllocator&) = delete;

	void* AllocateMemory(std::size_t size);
	std::size_t GetAllocationSize() const;

private:
	__declspec(allocator) void* MallocInternal(std::size_t size);

	void* memoryAddress_;
	std::size_t offsetPointer_;
	std::size_t blockSize_;
};