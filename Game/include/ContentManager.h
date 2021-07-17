#pragma once
#include "MonotonicAllocator.h"
#include <cstddef>

class ContentManager
{
public:
	ContentManager();

	void Load(const char* fileName);

private:
	MonotonicAllocator monoAllocator_;
};