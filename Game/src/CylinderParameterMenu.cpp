#include "CylinderParameterMenu.h"
#include "Shapes/Cylinder.h"
#include <imgui.h>
#include <iostream>

//TODO: Add checks for stuff like radius = 0

//CylinderParameterMenu::CylinderParameterMenu() : width_(300), height_(400), xbuf_{ "" }, ybuf_{ "" }, zbuf_{ "" }, theta_(0), phi_(0),
//radiusbuf_{ "" }, heightbuf_{ "" }, ObjectPendingCreation(false), namebuf_{ "" }, voltagebuf_{ "" }, detectionCyl_(false)
//{
//};

CylinderParameterMenu::CylinderParameterMenu() : width_(new double(0)), height_(new double(0)), x_(new double(0)), y_(new double(0)), z_(new double(0)), theta_(new double(0)), phi_(new double(0)), radius_(new double(0)),
ObjectPendingCreation(false), namebuf_{ "" }, voltage_(new double(0)), detectionCyl_(false)
{
};

CylinderParameterMenu::~CylinderParameterMenu()
{
	delete width_;
	delete height_;
	delete x_;
	delete y_;
	delete z_;
	delete radius_;
	delete voltage_;
	delete theta_;
	delete phi_;
}

void CylinderParameterMenu::Spawn(float xOffset, float yOffset)
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - 300, yOffset), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Cylinder parameters");


	//Plane Specific Parameters

	ImGui::Text("Dimensions (mm):");

	//How it should really look: for ... Create next menu as below, but for every input window set the item width to 180
	ImGui::SetNextItemWidth(180);
	ImGui::InputDouble("Radius", radius_, 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Height", height_, 0.01f, 1.0f, "%.4f");

	ImGui::Text("Translational offsets (mm):");
	ImGui::InputDouble("X", x_, 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Y", y_, 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Z", z_, 0.01f, 1.0f, "%.4f");

	ImGui::Text("Rotations (rad):");

	ImGui::InputDouble("Theta", theta_, 0.01f, 1.0f, "%.4f");
	ImGui::InputDouble("Phi", phi_, 0.01f, 1.0f, "%.4f");

	ImGui::Text("Voltage (V):");
	ImGui::InputDouble("Voltage", voltage_, 0.01f, 1.0f, "%.4f");

	ImGui::Text("Object name:");
	ImGui::InputText("Name", namebuf_, IM_ARRAYSIZE(namebuf_));

	//This code isn't how things should work. If ImGui returns true we don't store it! 
	//We need to send it somehow to the correct list (detectionList)
	ImGui::Checkbox("Detection shape", &detectionCyl_);

	if (ImGui::Button("Place object"))
	{
		ObjectPendingCreation = true;
	}

	ImGui::End();
}

Cylinder CylinderParameterMenu::GenerateObject()
{
	Cylinder cylinder;

	cylinder.x = *x_;
	cylinder.y = *y_;
	cylinder.z = *z_;
	cylinder.radius_ = *radius_;
	cylinder.height_ = *height_;
	cylinder.voltage = *voltage_;

	//These two should lose their underscore subscript, to keep a common naming convention
	cylinder.theta_ = *theta_;
	cylinder.phi_ = *phi_;

	//This copies the bytes located in namebuf_ into the shape's name below
	std::copy(std::begin(namebuf_), std::end(namebuf_), std::begin(cylinder.name));

	return cylinder;
}

void CylinderParameterMenu::ResetObject()
{
	ObjectPendingCreation = false;
}