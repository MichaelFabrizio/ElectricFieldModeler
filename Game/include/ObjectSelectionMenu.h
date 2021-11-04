#pragma once

#include "Shapes/Shape.h"
#include "CylinderParameterMenu.h"
#include "SphereParameterMenu.h"
#include "RPrismParameterMenu.h"
#include "HCylinderParameterMenu.h"

#include <vector>
#include <memory>

class ObjectSelectionMenu
{
public:
	ObjectSelectionMenu();
	~ObjectSelectionMenu();
	void GenerateWindow();
	void Process();
	void EmptyList(std::vector<std::unique_ptr<Shape>>& vector);
	void EmptyDetectors(std::vector<std::unique_ptr<Shape>>& vector);

private:
	CylinderParameterMenu* cylinderMenu;
	SphereParameterMenu* sphereMenu;
	RPrismParameterMenu* prismMenu;
	HCylinderParameterMenu* hCylinderMenu;

	int selected;
	float width;
	float height;

	std::vector<std::unique_ptr<Shape>> pendingShapes;
	std::vector<std::unique_ptr<Shape>> pendingDetectors;
};