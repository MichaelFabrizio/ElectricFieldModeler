#include "ObjectSelectionMenu.h"
#include "SphereParameterMenu.h"
#include <imgui.h>

#include <iostream>
#include <memory>

ObjectSelectionMenu::ObjectSelectionMenu() : width(300), height(200), selected(0)
{
	cylinderMenu = new CylinderParameterMenu();
	sphereMenu = new SphereParameterMenu();
	prismMenu = new RPrismParameterMenu();
	hCylinderMenu = new HCylinderParameterMenu();
};

ObjectSelectionMenu::~ObjectSelectionMenu()
{
	delete hCylinderMenu;
	delete prismMenu;
	delete sphereMenu;
	delete cylinderMenu;
};

void ObjectSelectionMenu::GenerateWindow()
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - width, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);

	ImGui::Begin("Object Selection");

	ImGui::RadioButton("Cylinder", &selected, 0); ImGui::SameLine();
	ImGui::RadioButton("Sphere", &selected, 1);
	ImGui::RadioButton("Rectangular Prism", &selected, 2);
	ImGui::RadioButton("Hollowed Cylinder", &selected, 3);

	ImGui::End();
}

void ObjectSelectionMenu::Process()
{
	switch (selected)
	{
	case 0:
		cylinderMenu->Spawn(0, height);

		if (cylinderMenu->ObjectPendingCreation)
		{
			Cylinder cylinder = cylinderMenu->GenerateObject();
			std::unique_ptr<Cylinder> ptr = std::make_unique<Cylinder>(cylinder);

			if (cylinderMenu->detectionCyl_)
			{
				pendingDetectors.push_back(std::move(ptr));
				cylinderMenu->ResetObject();
			}
			else
			{
				pendingShapes.push_back(std::move(ptr));
				cylinderMenu->ResetObject();
			}
		}
		break;
	case 1:
		sphereMenu->Spawn(0, height);

		if (sphereMenu->ObjectPendingCreation)
		{
			Sphere sphere = sphereMenu->GenerateObject();
			std::unique_ptr<Sphere> ptr = std::make_unique<Sphere>(sphere);
			pendingShapes.push_back(std::move(ptr));
			sphereMenu->ResetObject();
		}
		break;
	case 2:
		prismMenu->Spawn(0, height);

		if (prismMenu->ObjectPendingCreation)
		{
			RPrism prism = prismMenu->GenerateObject();
			std::unique_ptr<RPrism> ptr = std::make_unique<RPrism>(prism);
			pendingShapes.push_back(std::move(ptr));
			prismMenu->ResetObject();
		}
		break;
	case 3:
		hCylinderMenu->Spawn(0, height);
		if (hCylinderMenu->ObjectPendingCreation)
		{
			HCylinder cylinder = hCylinderMenu->GenerateObject();
			std::unique_ptr<HCylinder> ptr = std::make_unique<HCylinder>(cylinder);
			pendingShapes.push_back(std::move(ptr));
			hCylinderMenu->ResetObject();
		}
		break;
	}
}

void ObjectSelectionMenu::EmptyList(std::vector<std::unique_ptr<Shape>>& ListToAdd)
{
	ListToAdd = std::move(pendingShapes);
	pendingShapes.clear();
}

void ObjectSelectionMenu::EmptyDetectors(std::vector<std::unique_ptr<Shape>>& ListToAdd)
{
	ListToAdd = std::move(pendingDetectors);
	pendingDetectors.clear();
}