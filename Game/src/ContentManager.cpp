#include "ContentManager.h"
#include "MonotonicAllocator.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ContentManager::ContentManager()
{
}

void ContentManager::Load(const char* fileName)
{
	int x, y, n;
	unsigned char* data = stbi_load(fileName, &x, &y, &n, 0);

	


	stbi_image_free(data);
}