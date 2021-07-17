#pragma once
#include "Shapes/Shape.h"
#include <vector>
#include <memory>
#include <string>

class Laplacian
{
public:
	Laplacian(int iterations, int gridX, int gridY, int gridZ, float gridScaling);
	~Laplacian();
	void Calculate(std::vector<std::unique_ptr<Shape>>& objectList);
	void DebugPrint();
	void ExportCSV();
private:
	void CheckOccupation(std::vector<std::unique_ptr<Shape>>& objectList);

	float*** grid;
	float**** electricField;
	bool*** occupiedInternal;
	float w;
	float threshold_;
	float iterations_;
	int gridX_;
	int gridY_;
	int gridZ_;
	float gridScaling_;
	std::string path_;
	std::string eFieldPath_;

};