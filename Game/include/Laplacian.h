#pragma once
#include "Shapes/Shape.h"
#include <vector>
#include <memory>
#include <string>

class Laplacian
{
public:
	Laplacian();
	~Laplacian();
	bool BuildGrid(int iterations, int gridX, int gridY, int gridZ, double gridScaling);
	void Calculate(std::vector<std::unique_ptr<Shape>>& objectList);
	void DebugPrint();
	void ExportCSV();
	double****& GetEField();
	bool CheckEField();
	double GetGridScaling();
	inline int GetGridX() { return gridX_; }
	inline int GetGridY() { return gridY_; }
	inline int GetGridZ() { return gridZ_; }

	bool successful;
private:
	void CheckOccupation(std::vector<std::unique_ptr<Shape>>& objectList);

	double*** grid;
	double**** electricField;
	bool*** occupiedInternal;
	float w;
	float threshold_;
	float iterations_;
	int gridX_;
	int gridY_;
	int gridZ_;
	double gridScaling_;
	std::string path_;
	std::string eFieldPath_;

	bool gridConstructed_;
};