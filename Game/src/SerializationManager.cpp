#include "SerializationManager.h"
#include "Shapes/Cylinder.h"
#include "Shapes/HCylinder.h"
#include "Shapes/RPrism.h"
#include <imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>

SerializationManager::SerializationManager() : filebuf_{ "" }, savefilebuf_{ "" } 
{
}

void SerializationManager::BuildWindow(const ImGuiViewport& viewport, std::vector<std::unique_ptr<Shape>>& objectList)
{
	ImGui::SetNextWindowPos(ImVec2(0, viewport.Size.y - 150), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);

	int windowflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Load/Save Menu", NULL, windowflags);

	ImGui::Text("Load object data from file");
	ImGui::InputText("##label2", filebuf_, IM_ARRAYSIZE(filebuf_)); ImGui::SameLine();
	if (ImGui::SmallButton("Load"))
		Load(objectList);

	ImGui::Text("Save object data to file");
	ImGui::InputText("##label", savefilebuf_, IM_ARRAYSIZE(savefilebuf_)); ImGui::SameLine();
	if (ImGui::SmallButton("Save"))
		Save(objectList);

	ImGui::End();
}

void SerializationManager::Load(std::vector<std::unique_ptr<Shape>>& ListToAdd)
{
	filePath_ = filebuf_;
	std::replace(filePath_.begin(), filePath_.end(), '\\', '/');

	std::ifstream input;
	input.open(filePath_);

	if (!input.is_open())
	{
		std::cout << "File failed to open" << '\n';
	}
	else
	{
		for (std::string line; std::getline(input, line); )
		{
			if (line == "Cylinder")
			{
				shapeType = SerializedShape::CylinderType;
			}
			else if (line == "HCylinder")
			{
				shapeType = SerializedShape::HCylinderType;
			}
			else if (line == "RPrism")
			{
				shapeType = SerializedShape::RPrismType;
			}
			else
			{
				std::cout << "Invalid save file shape entry" << '\n';
			}

			switch (shapeType)
			{
			case SerializedShape::CylinderType:
			{
				Cylinder cylinder;
				std::string subLine[9];
				for (size_t i = 0; i < 9; i++)
				{
					std::getline(input, subLine[i]);
				}

				cylinder.x = (float)stof(subLine[0]);
				cylinder.y = (float)stof(subLine[1]);
				cylinder.z = (float)stof(subLine[2]);
				cylinder.voltage = (float)stof(subLine[3]);
				cylinder.radius_ = (float)stof(subLine[4]);
				cylinder.height_ = (float)stof(subLine[5]);
				cylinder.theta_ = (float)stof(subLine[6]);
				cylinder.phi_ = (float)stof(subLine[7]);

				std::copy(std::begin(subLine[8]), std::end(subLine[8]), std::begin(cylinder.name));

				std::unique_ptr<Cylinder> ptr = std::make_unique<Cylinder>(cylinder);
				pendingShapes.push_back(std::move(ptr));
				break;
			}
			case SerializedShape::HCylinderType:
			{
				HCylinder hcylinder;
				std::string subLine[11];
				for (size_t i = 0; i < 11; i++)
				{
					std::getline(input, subLine[i]);
				}
				hcylinder.x = (float)stof(subLine[0]);
				hcylinder.y = (float)stof(subLine[1]);
				hcylinder.z = (float)stof(subLine[2]);
				hcylinder.voltage = (float)stof(subLine[3]);
				hcylinder.innerRadius = (float)stof(subLine[4]);
				hcylinder.outerRadius = (float)stof(subLine[5]);
				hcylinder.height = (float)stof(subLine[6]);
				hcylinder.theta = (float)stof(subLine[7]);
				hcylinder.phi = (float)stof(subLine[8]);
				std::copy(std::begin(subLine[9]), std::end(subLine[9]), std::begin(hcylinder.name));

				if (subLine[10] == "True")
				{
					hcylinder.emptyAttachment = true;
					Cylinder emptyCyl;
					std::string emptyCylParse[8];

					for (size_t i = 0; i < 8; i++)
					{
						std::getline(input, emptyCylParse[i]);
					}

					emptyCyl.x = (float)stof(emptyCylParse[0]);
					emptyCyl.y = (float)stof(emptyCylParse[1]);
					emptyCyl.z = (float)stof(emptyCylParse[2]);
					emptyCyl.voltage = (float)stof(emptyCylParse[3]);
					emptyCyl.radius_ = (float)stof(emptyCylParse[4]);
					emptyCyl.height_ = (float)stof(emptyCylParse[5]);
					emptyCyl.theta_ = (float)stof(emptyCylParse[6]);
					emptyCyl.phi_ = (float)stof(emptyCylParse[7]);

					hcylinder.empty = emptyCyl;
				}

				std::unique_ptr<HCylinder> ptr = std::make_unique<HCylinder>(hcylinder);
				pendingShapes.push_back(std::move(ptr));
				break;
			}
			case SerializedShape::RPrismType:
			{
				RPrism rprism;
				std::string subLine[10];
				
				for (size_t i = 0; i < 10; i++)
				{
					std::getline(input, subLine[i]);
				}

				rprism.x = (float)stof(subLine[0]);
				rprism.y = (float)stof(subLine[1]);
				rprism.z = (float)stof(subLine[2]);
				rprism.voltage = (float)stof(subLine[3]);
				rprism.length = (float)stof(subLine[4]);
				rprism.width = (float)stof(subLine[5]);
				rprism.height = (float)stof(subLine[6]);
				rprism.theta = (float)stof(subLine[7]);
				rprism.phi = (float)stof(subLine[8]);
				std::copy(std::begin(subLine[9]), std::end(subLine[9]), std::begin(rprism.name));

				std::unique_ptr<RPrism> ptr = std::make_unique<RPrism>(rprism);
				pendingShapes.push_back(std::move(ptr));

				break;
			}
			default:
				break;
			}
		}
	}

	input.close();
	ListToAdd = std::move(pendingShapes);
	pendingShapes.clear();
}

void SerializationManager::Save(std::vector<std::unique_ptr<Shape>>& objectList)
{
	filePath_ = savefilebuf_;

	std::replace(filePath_.begin(), filePath_.end(), '\\', '/');

	std::ofstream output;
	output.open(filePath_);

	if (!output.is_open())
	{
		std::cout << "Save file procedure failed, DEBUG: SerializationManager class" << '\n';
	}
	else
	{
		for (size_t i = 0; i < objectList.size(); i++)
		{
			if ((*objectList[i]).shapeType == 0)
			{
				Cylinder& cylinder = static_cast<Cylinder&>((*objectList[i]));

				output << "Cylinder" << '\n';
				output << cylinder.x << '\n';
				output << cylinder.y << '\n';
				output << cylinder.z << '\n';
				output << cylinder.voltage << '\n';
				output << cylinder.radius_ << '\n';
				output << cylinder.height_ << '\n';
				output << cylinder.theta_ << '\n';
				output << cylinder.phi_ << '\n';
				output << cylinder.name << '\n';
			}

			else if ((*objectList[i]).shapeType == 1)
			{
				HCylinder& hcylinder = static_cast<HCylinder&>((*objectList[i]));

				output << "HCylinder" << '\n';
				output << hcylinder.x << '\n';
				output << hcylinder.y << '\n';
				output << hcylinder.z << '\n';
				output << hcylinder.voltage << '\n';
				output << hcylinder.innerRadius << '\n';
				output << hcylinder.outerRadius << '\n';
				output << hcylinder.height << '\n';
				output << hcylinder.theta << '\n';
				output << hcylinder.phi << '\n';
				output << hcylinder.name << '\n';

				if (hcylinder.emptyAttachment == true)
				{
					output << "True" << '\n';
					output << hcylinder.empty.x << '\n';
					output << hcylinder.empty.y << '\n';
					output << hcylinder.empty.z << '\n';
					output << hcylinder.empty.voltage << '\n';
					output << hcylinder.empty.radius_ << '\n';
					output << hcylinder.empty.height_ << '\n';
					output << hcylinder.empty.theta_ << '\n';
					output << hcylinder.empty.phi_ << '\n';
				}
				else
					output << "False" << '\n';
			}
			else if ((*objectList[i]).shapeType == 2)
			{
				//rprism.x = (float)stof(subLine[0]);
				//rprism.y = (float)stof(subLine[1]);
				//rprism.z = (float)stof(subLine[2]);
				//rprism.voltage = (float)stof(subLine[3]);
				//rprism.length = (float)stof(subLine[4]);
				//rprism.width = (float)stof(subLine[5]);
				//rprism.height = (float)stof(subLine[6]);
				//rprism.theta = (float)stof(subLine[7]);
				//rprism.phi = (float)stof(subLine[8]);

				RPrism& rprism = static_cast<RPrism&>((*objectList[i]));

				output << "RPrism" << '\n';
				output << rprism.x << '\n';
				output << rprism.y << '\n';
				output << rprism.z << '\n';
				output << rprism.voltage << '\n';
				output << rprism.length << '\n';
				output << rprism.width << '\n';
				output << rprism.height << '\n';
				output << rprism.theta << '\n';
				output << rprism.phi << '\n';
				output << rprism.name << '\n';
			}
		}
		std::cout << "File saved" << '\n';
	}

	output.close();
}