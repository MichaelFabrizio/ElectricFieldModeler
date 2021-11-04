#include "ParticleWindow.h"
#include "ParticleInformationBundle.h"
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <shlobj.h>
#include <windows.h>

#include <iomanip>
#include <map>
#include <cmath>

ParticleWindow::ParticleWindow(Laplacian& laplacian) : energyBuf_{ "" }, chargeBuf_{ "" }, massBuf_{ "" }, timestepBuf_{ "" }, collimatordistBuf_{ "" },
estimatedtofBuf_{ "" }, collimatorradiusBuf_{ "" }, numberOfParticlesBuf_{ "" }, laplacian_(laplacian), collXCenter_{ "" },
collYCenter_{ "" }, collZCenter_{ "" }, charge_(0), mass_(0), timeStep_(0), estimatedTof_(0), collimatorDist_(0), collimatorRadius_(0), bundle_()
{
	bundle_ = new ParticleInformationBundle();
}

ParticleWindow::~ParticleWindow()
{
	delete bundle_;
}

void ParticleWindow::BuildWindow(const ImGuiViewport& viewport, std::vector<std::unique_ptr<Shape>>& detectionList)
{

	ImGui::SetNextWindowPos(ImVec2(0, viewport.Size.y - 240), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(530, 240), ImGuiCond_FirstUseEver);

	int windowflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Trajectory parameters", NULL, windowflags);

	ImGui::InputDouble("Backscatter energy (eV)", bundle_->GetEnergy(), 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Charge (e)", bundle_->GetCharge(), 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Mass (amu)", bundle_->GetMass(), 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Timestep (ns)", bundle_->GetTimestep(), 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Estimated ToF (ns)", bundle_->GetEstimatedTof(), 0.01f, 1.0f, "%.4f");

	//Takes all of the values entered into the trajectory parameters window and stores them as char[16] arrays

	ImGui::InputText("Collimator distance (mm)", collimatordistBuf_, IM_ARRAYSIZE(collimatordistBuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Collimator radius (mm)", collimatorradiusBuf_, IM_ARRAYSIZE(collimatorradiusBuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Collimator X offset", collXCenter_, IM_ARRAYSIZE(collXCenter_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Collimator Y offset", collYCenter_, IM_ARRAYSIZE(collYCenter_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Collimator Z offset", collZCenter_, IM_ARRAYSIZE(collZCenter_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Particle count", numberOfParticlesBuf_, IM_ARRAYSIZE(numberOfParticlesBuf_), ImGuiInputTextFlags_CharsDecimal);


	if (ImGui::Button("Model trajectories"))
		ModelTrajectories(detectionList);

	ImGui::End();
}

void ParticleWindow::ModelTrajectories(std::vector<std::unique_ptr<Shape>>& detectionList)
{
	if (!laplacian_.CheckEField())
	{
		std::cout << "No potential has been calculated yet" << '\n';
		return;
	}
	double****& efield = laplacian_.GetEField();

	//These fields really don't belong here, should be produced when CORE asks for the values, AKA when the ModelTrajectory button is clicked
	double energy = atof(energyBuf_) * 1.6022e-19;
	double charge = atof(chargeBuf_) * 1.6022e-19;
	double mass = atof(massBuf_) * 1.66054e-27;
	double timestep = atof(timestepBuf_) * 1.0e-9;
	double collimatorDist = atof(collimatordistBuf_);
	double estimatedToF = atof(estimatedtofBuf_) * 1.0e-9;
	double collimatorRadius = atof(collimatorradiusBuf_);
	int particleCount = atoi(numberOfParticlesBuf_);
	double collXOffset = atof(collXCenter_);
	double collYOffset = atof(collYCenter_);
	double collZOffset = atof(collZCenter_);

	if (timestep == 0 || estimatedToF == 0)
	{
		std::cout << "No user entered timestep or estimated ToF" << '\n';
		return;
	}

	int iterations = (int)(estimatedToF / timestep);

	double vPeak = sqrt(energy * 2.0 / mass);

	if (energy == 0 || mass == 0)
	{
		std::cout << "Missing backscatter energy or mass inputs" << '\n';
		return;
	}
	//Add checks for collOffsets, make sure they are within the grid[i+2] etc boundaries
	if (collimatorDist == 0 || collimatorRadius == 0)
	{
		std::cout << "Missing collimator data" << "\n";
	}

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	std::normal_distribution<> rSpread{ 0, collimatorRadius / 3.0f};
	std::normal_distribution<> vSpread{ vPeak, vPeak * 0.2 };

	std::vector<std::vector<std::vector<double>>> particlePositionList;

	for (size_t i = 0; i < particleCount; i++)
	{
		double vertRadial = rSpread(gen);
		double horizRadial = rSpread(gen);
		while (!(sqrt(pow(vertRadial, 2) + pow(horizRadial, 2)) <= collimatorRadius))
		{
			vertRadial = rSpread(gen);
			horizRadial = rSpread(gen);
		}

		double xInitial = collXOffset + horizRadial;
		double zInitial = collZOffset + vertRadial;

		double speed = vSpread(gen);
		while (speed <= 0)
		{
			speed = vSpread(gen);
		}

		std::cout << speed << "m/s" << '\n';
		speed = speed * laplacian_.GetGridScaling() * 1000.0;

		std::cout << "Speed test:" << speed << '\n';

		double hypotenuse = sqrt(pow(horizRadial, 2) + pow(vertRadial, 2) + pow(collimatorDist, 2));
		double hypotenuseXY = sqrt(pow(hypotenuse, 2) - pow(vertRadial, 2));

		double vz = speed * vertRadial / hypotenuse;
		double vx = speed * horizRadial / hypotenuseXY;
		double vy = speed * collimatorDist / hypotenuseXY;

		std::vector<std::vector<double>> rVecList;
		std::vector<double> initialConditions = { xInitial, collYOffset, zInitial, vx, vy, vz };
		rVecList.push_back(initialConditions);

		double timeAccumulated = 0;

		for (size_t i = 0; i < iterations; i++)
		{
			timeAccumulated += timestep;

			std::vector<double> k1;
			std::vector<double> k2;
			std::vector<double> k3;
			std::vector<double> k4;

			bool boundsCheck = false;

			InterpolateEField(rVecList[i][0], rVecList[i][1], rVecList[i][2], efield, charge, mass, rVecList[i][3], rVecList[i][4], rVecList[i][5], k1, boundsCheck);
			if (boundsCheck)
			{
				std::cout << "[ERROR] Particle exited the grid space" << '\n';
				return;
			}

			CalcK2(k1, timestep, rVecList[i][0], rVecList[i][1], rVecList[i][2], rVecList[i][3], rVecList[i][4], rVecList[i][5], efield, charge, mass, k2, boundsCheck);
			if (boundsCheck)
			{
				std::cout << "[ERROR] Particle exited the grid space" << '\n';
				return;
			}
			CalcK3(k2, timestep, rVecList[i][0], rVecList[i][1], rVecList[i][2], rVecList[i][3], rVecList[i][4], rVecList[i][5], efield, charge, mass, k3, boundsCheck);
			if (boundsCheck)
			{
				std::cout << "[ERROR] Particle exited the grid space" << '\n';
				return;
			}
			CalcK4(k3, timestep, rVecList[i][0], rVecList[i][1], rVecList[i][2], rVecList[i][3], rVecList[i][4], rVecList[i][5], efield, charge, mass, k4, boundsCheck);
			if (boundsCheck)
			{
				std::cout << "[ERROR] Particle exited the grid space" << '\n';
				return;
			}

			double xUpdated = rVecList[i][0] + (timestep / 6.0) * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
			double yUpdated = rVecList[i][1] + (timestep / 6.0) * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
			double zUpdated = rVecList[i][2] + (timestep / 6.0) * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
			double vxUpdated = rVecList[i][3] + (timestep / 6.0) * (k1[3] + 2 * k2[3] + 2 * k3[3] + k4[3]);
			double vyUpdated = rVecList[i][4] + (timestep / 6.0) * (k1[4] + 2 * k2[4] + 2 * k3[4] + k4[4]);
			double vzUpdated = rVecList[i][5] + (timestep / 6.0) * (k1[5] + 2 * k2[5] + 2 * k3[5] + k4[5]);

			std::vector<double> updatedVec = { xUpdated, yUpdated, zUpdated, vxUpdated, vyUpdated, vzUpdated };
			rVecList.push_back(updatedVec);

			for (size_t i = 0; i < detectionList.size(); i++)
			{
				if ((*detectionList[i]).CheckPointForOccupation(xUpdated, yUpdated, zUpdated, laplacian_.GetGridScaling()))
				{
					std::cout << "Particle inside detector, should break loop" << '\n';
					std::cout << "TOF found: " << timeAccumulated << '\n';
					return;
				}
			}
		}

		particlePositionList.push_back(rVecList);
	}

	ParticleWindow::SaveTrajectoryContour(particlePositionList);
}

void ParticleWindow::InterpolateEField(double xPosition, double yPosition, double zPosition, double****& efield, double charge, double mass,
	double vx, double vy, double vz, std::vector<double>& vecArray, bool& boundsCheck)
{
	double upperXraw = ceil(xPosition);
	double lowerXraw = floor(xPosition);
	double upperYraw = ceil(yPosition);
	double lowerYraw = floor(yPosition);
	double upperZraw = ceil(zPosition);
	double lowerZraw = floor(zPosition);

	int upperX = (int)upperXraw;
	int lowerX = (int)lowerXraw;
	int upperY = (int)upperYraw;
	int lowerY = (int)lowerYraw;
	int upperZ = (int)upperZraw;
	int lowerZ = (int)lowerZraw;

	if (upperX > (laplacian_.GetGridX() - 1) || lowerX < 0 || upperY > (laplacian_.GetGridY() - 1) || lowerY < 0 || upperZ > (laplacian_.GetGridZ() - 1) || lowerZ < 0)
	{
		boundsCheck = true;
		return;
	}

	double weightXHigh = (xPosition - lowerXraw) / 4.0;
	double weightXLow = (upperXraw - xPosition) / 4.0;
	double weightYHigh = (yPosition - lowerYraw) / 4.0;
	double weightYLow = (upperYraw - yPosition) / 4.0;
	double weightZHigh = (zPosition - lowerZraw) / 4.0;
	double weightZLow = (upperZraw - zPosition) / 4.0;

	double efieldx =
		weightXHigh * (efield[upperX][upperY][upperZ][0] + efield[upperX][upperY][lowerZ][0] +
		efield[upperX][lowerY][upperZ][0] + efield[upperX][lowerY][lowerZ][0])
		+
		weightXLow * (efield[lowerX][upperY][upperZ][0] + efield[lowerX][upperY][lowerZ][0] +
		efield[lowerX][lowerY][upperZ][0] + efield[lowerX][lowerY][lowerZ][0]);

	double efieldy =
		weightYHigh * (efield[upperX][upperY][upperZ][1] + efield[upperX][upperY][lowerZ][1] +
		efield[lowerX][upperY][upperZ][1] + efield[lowerX][upperY][lowerZ][1])
		+
		weightYLow * (efield[upperX][lowerY][upperZ][1] + efield[upperX][lowerY][lowerZ][1] +
		efield[lowerX][lowerY][upperZ][1] + efield[lowerX][lowerY][lowerZ][1]);

	double efieldz =
		weightZHigh * (efield[upperX][upperY][upperZ][2] + efield[upperX][lowerY][upperZ][2] +
		efield[lowerX][upperY][upperZ][2] + efield[lowerX][lowerY][upperZ][2])
		+
		weightZLow * (efield[upperX][upperY][lowerZ][2] + efield[upperX][lowerY][lowerZ][2] +
		efield[lowerX][upperY][lowerZ][2] + efield[lowerX][lowerY][lowerZ][2]);

	double scaling = laplacian_.GetGridScaling();

	double accelx = scaling * 1000.0 * efieldx * charge / mass;
	double accely = scaling * 1000.0 * efieldy * charge / mass;
	double accelz = scaling * 1000.0 * efieldz * charge / mass;

	vecArray.push_back(vx);
	vecArray.push_back(vy);
	vecArray.push_back(vz);
	vecArray.push_back(accelx);
	vecArray.push_back(accely);
	vecArray.push_back(accelz);
}

void ParticleWindow::CalcK2(std::vector<double> k1, double timestep, double xInitial, double yInitial, double zInitial, double vxInitial,
	double vyInitial, double vzInitial, double****& efield, double charge, double mass, std::vector<double>& k2Vec, bool& boundsCheck)
{
	double xinput = xInitial + timestep * k1[0] / 2.0;
	double yinput = yInitial + timestep * k1[1] / 2.0;
	double zinput = zInitial + timestep * k1[2] / 2.0;
	double vxinput = vxInitial + timestep * k1[3] / 2.0;
	double vyinput = vyInitial + timestep * k1[4] / 2.0;
	double vzinput = vzInitial + timestep * k1[5] / 2.0;

	InterpolateEField(xinput, yinput, zinput, efield, charge, mass, vxinput, vyinput, vzinput, k2Vec, boundsCheck);
}

void ParticleWindow::CalcK3(std::vector<double> k2, double timestep, double xInitial, double yInitial, double zInitial, double vxInitial,
	double vyInitial, double vzInitial, double****& efield, double charge, double mass, std::vector<double>& k3Vec, bool& boundsCheck)
{
	double xinput = xInitial + timestep * k2[0] / 2.0;
	double yinput = yInitial + timestep * k2[1] / 2.0;
	double zinput = zInitial + timestep * k2[2] / 2.0;
	double vxinput = vxInitial + timestep * k2[3] / 2.0;
	double vyinput = vyInitial + timestep * k2[4] / 2.0;
	double vzinput = vzInitial + timestep * k2[5] / 2.0;

	InterpolateEField(xinput, yinput, zinput, efield, charge, mass, vxinput, vyinput, vzinput, k3Vec, boundsCheck);
}

void ParticleWindow::CalcK4(std::vector<double> k3, double timestep, double xInitial, double yInitial, double zInitial, double vxInitial,
	double vyInitial, double vzInitial, double****& efield, double charge, double mass, std::vector<double>& k4Vec, bool& boundsCheck)
{
	double xinput = xInitial + timestep * k3[0];
	double yinput = yInitial + timestep * k3[1];
	double zinput = zInitial + timestep * k3[2];
	double vxinput = vxInitial + timestep * k3[3];
	double vyinput = vyInitial + timestep * k3[4];
	double vzinput = vzInitial + timestep * k3[5];

	InterpolateEField(xinput, yinput, zinput, efield, charge, mass, vxinput, vyinput, vzinput, k4Vec, boundsCheck);
}

void ParticleWindow::SaveTrajectoryContour(std::vector<std::vector<std::vector<double>>>& particlePositionList)
{
	std::ofstream output;
	std::string path_;

	wchar_t my_documents[1024];
	HRESULT result = SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (SUCCEEDED(result))
	{
		char str[1024];
		wcstombs(str, my_documents, 1023);

		path_ = str;
		std::replace(path_.begin(), path_.end(), '\\', '/');
		path_.append("/trajectory.csv");
	}
	else
	{
		std::cout << "Error grabbing documents folder path" << '\n';
		return;
	}

	output.open(path_);

	auto particleListSize = particlePositionList.size();
	for (size_t i = 0; i < particleListSize - 1; i++)
	{
		auto rVecList = particlePositionList[i];
		auto size = rVecList.size();
		for (size_t j = 0; j < size - 1; j++)
		{
			output << "\"{" << rVecList[j][0] << "," << rVecList[j][1] << "}\",";
		}
		output << "\"{" << rVecList[size-1][0] << "," << rVecList[size-1][1] << "}\"\n";

		//output << "}\",";
	}

	auto rVecList = particlePositionList[particleListSize - 1];
	auto size = rVecList.size();

	//output << "\"";
	for (size_t j = 0; j < size - 1; j++)
	{
		output << "\"{" << rVecList[j][0] << "," << rVecList[j][1] << "}\",";
	}
	output << "\"{" << rVecList[size - 1][0] << "," << rVecList[size - 1][1] << "}\"";

	//output << "\"";


	output.close();
	std::cout << "Trajectory saved" << std::endl;
}