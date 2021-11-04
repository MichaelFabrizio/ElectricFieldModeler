#pragma once
#include <imgui.h>
#include "Laplacian.h"
#include "ParticleInformationBundle.h"

class ParticleWindow
{
public:
	ParticleWindow(Laplacian& laplacian);
	~ParticleWindow();
	void BuildWindow(const ImGuiViewport& viewport, std::vector<std::unique_ptr<Shape>>& detectionList);

private:
	void ModelTrajectories(std::vector<std::unique_ptr<Shape>>& detectionList);
	void InterpolateEField(double xPosition, double yPosition, double zPosition, double****& efield, double charge, double mass,
		double vx, double vy, double vz, std::vector<double>& vecArray, bool& boundsCheck);
	void CalcK2(std::vector<double> k1, double timestep, double xInitial, double yInitial, double zInitial, double vxInitial,
		double vyInitial, double vzInitial, double****& efield, double charge, double mass, std::vector<double>& k2Vec, bool& boundsCheck);

	void CalcK3(std::vector<double> k2, double timestep, double xInitial, double yInitial, double zInitial, double vxInitial,
		double vyInitial, double vzInitial, double****& efield, double charge, double mass, std::vector<double>& k3Vec, bool& boundsCheck);

	void CalcK4(std::vector<double> k3, double timestep, double xInitial, double yInitial, double zInitial, double vxInitial,
		double vyInitial, double vzInitial, double****& efield, double charge, double mass, std::vector<double>& k4Vec, bool& boundsCheck);

	void SaveTrajectoryContour(std::vector<std::vector<std::vector<double>>>& rVecList);

	char energyBuf_[16];
	char chargeBuf_[16];
	char massBuf_[16];
	char timestepBuf_[16];
	char collimatordistBuf_[16];
	char collimatorradiusBuf_[16];
	char estimatedtofBuf_[16];
	char numberOfParticlesBuf_[16];
	char collXCenter_[16];
	char collYCenter_[16];
	char collZCenter_[16];
	Laplacian& laplacian_;

	double* energy_;
	double charge_;
	double mass_;
	double timeStep_;
	double collimatorDist_;
	double collimatorRadius_;
	double estimatedTof_;
	double numberOfParticles_;

	ParticleInformationBundle* bundle_;

	//double collXCenter_;
	//double collYCenter_;
	//double collZCenter_;
};