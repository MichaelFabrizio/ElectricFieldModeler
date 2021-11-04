#pragma once

class ParticleInformationBundle
{
public:
	//Constructor and Destructor (Handles creation and destruction of THIS class)
	ParticleInformationBundle();
	~ParticleInformationBundle();

	//Basic functions which can be called to get the underlying values in the class.

	inline double* GetEnergy()          const { return energy; }
	inline double* GetCharge()          const { return charge; }
	inline double* GetMass()            const { return mass; }
	inline double* GetTimestep()        const { return timeStep; }
	inline double* GetEstimatedTof()    const { return estimatedTof; }
	inline double* GetNumOfParticles()  const { return numberOfParticles; }
private:
	double* energy;
	double* charge;
	double* mass;
	double* timeStep;
	double* estimatedTof;
	double* numberOfParticles;

	//These are described in the collimator triangle diagram in the manual, hopefully soon.

	double collimatorDist;
	double collimatorRadius;

	//Collinder X,Y, and Z origin points. This value is the starting point of the particle!!!

	double collXCenter_;
	double collYCenter_;
	double collZCenter_;
};