#pragma once
#include "Shapes/Shape.h"

#include <imgui.h>
#include <string>
#include <vector>
#include <memory>

enum SerializedShape
{
	CylinderType,
	HCylinderType,
	RPrismType,
	SphereType
};

class SerializationManager
{
public:
	SerializationManager();
	void Save(std::vector<std::unique_ptr<Shape>>& objectList);
	void Load(std::vector<std::unique_ptr<Shape>>& ListToAdd);
	void BuildWindow(const ImGuiViewport& viewport, std::vector<std::unique_ptr<Shape>>& objectList);
private:
	char filebuf_[128];
	char savefilebuf_[128];
	std::string filePath_;
	SerializedShape shapeType;
	std::vector<std::unique_ptr<Shape>> pendingShapes;
};