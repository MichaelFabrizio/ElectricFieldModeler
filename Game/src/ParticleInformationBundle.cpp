#include "ParticleInformationBundle.h"

ParticleInformationBundle::ParticleInformationBundle() : energy(new double(0)), charge(new double(0)), mass(new double(0)), timeStep(new double(0)), estimatedTof(new double(0)),
numberOfParticles(new double(0)), collimatorDist(0), collimatorRadius(0), collXCenter_(0), collYCenter_(0), collZCenter_(0)
{

}

ParticleInformationBundle::~ParticleInformationBundle() 
{
	delete energy;
	delete charge;
	delete mass;
	delete timeStep;
	delete estimatedTof;
	delete numberOfParticles;
}