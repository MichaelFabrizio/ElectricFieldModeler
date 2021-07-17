#pragma once
#include <random>

class MCIntegrator
{
public:
	MCIntegrator();
	void Integrate();

private:
	std::random_device rd_;
};