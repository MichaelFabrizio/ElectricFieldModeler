#include <iostream>

#include <SDL.h>
#include "Core.h"
#include "MCIntegrator.h"

#include "Quad.h"

int main(int argc, char* argv[])
{
	Core* core = new Core();

	core->Init();

	core->Run();

	delete core;

	return 0;
}