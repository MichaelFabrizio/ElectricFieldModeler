#include "CylinderParameterMenu.h"
#include "Shapes/Cylinder.h"
#include <imgui.h>
#include <iostream>

//TODO: Add checks for stuff like radius = 0

CylinderParameterMenu::CylinderParameterMenu() : width_(300), height_(400), xbuf_{ "" }, ybuf_{ "" }, zbuf_{ "" }, theta_(0), phi_(0),
radiusbuf_{ "" }, heightbuf_{ "" }, ObjectPendingCreation(false), namebuf_{ "" }, voltagebuf_{ "" }
{
};

void CylinderParameterMenu::Spawn(float xOffset, float yOffset)
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - width_, yOffset), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width_, height_), ImGuiCond_FirstUseEver);

	ImGui::Begin("Cylinder parameters");
	ImGui::Text("Offsets (mm):");
	ImGui::InputText("X", xbuf_, IM_ARRAYSIZE(xbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Y", ybuf_, IM_ARRAYSIZE(ybuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Z", zbuf_, IM_ARRAYSIZE(zbuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Rotations (radians):");

	ImGui::SliderFloat("Theta", &theta_, 0.0f, 6.283185307f);
	ImGui::SliderFloat("Phi", &phi_, 0.0f, 6.283185307f);

	//Plane Specific Parameters

	ImGui::Text("Dimensions (mm):");
	ImGui::InputText("Radius", radiusbuf_, IM_ARRAYSIZE(radiusbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Height", heightbuf_, IM_ARRAYSIZE(heightbuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Voltage (V):");
	ImGui::InputText("Voltage", voltagebuf_, IM_ARRAYSIZE(voltagebuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Object name:");
	ImGui::InputText("Name", namebuf_, IM_ARRAYSIZE(namebuf_));

	if (ImGui::Button("Place object"))
	{
		ObjectPendingCreation = true;
	}

	ImGui::End();
}

Cylinder CylinderParameterMenu::GenerateObject()
{
	Cylinder cylinder;

	cylinder.x = (float)atof(xbuf_);
	cylinder.y = (float)atof(ybuf_);
	cylinder.z = (float)atof(zbuf_);
	cylinder.radius_ = (float)atof(radiusbuf_);
	cylinder.height_ = (float)atof(heightbuf_);
	cylinder.voltage = (float)atof(voltagebuf_);
	cylinder.theta_ = theta_;
	cylinder.phi_ = phi_;
	std::copy(std::begin(namebuf_), std::end(namebuf_), std::begin(cylinder.name));

	return cylinder;
}

void CylinderParameterMenu::ResetObject()
{
	ObjectPendingCreation = false;
}