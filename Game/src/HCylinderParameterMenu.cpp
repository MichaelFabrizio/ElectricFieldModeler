#include "HCylinderParameterMenu.h"

#include "Shapes/HCylinder.h"
#include <imgui.h>
#include <iostream>

HCylinderParameterMenu::HCylinderParameterMenu() : width_(300), height_(400), xbuf_{ "" }, ybuf_{ "" }, zbuf_{ "" }, theta_(0), phi_(0),
radiusinnerbuf_{ "" }, radiusouterbuf_{ "" }, heightbuf_{ "" }, ObjectPendingCreation(false), namebuf_{ "" }, voltagebuf_{ "" },
attachEmpty_(false), xbufempty_{ "" }, ybufempty_{ "" }, zbufempty_{ "" }, radiusbufempty_{ "" }, heightbufempty_{""},
thetaempty_(0), phiempty_(0)
{
};

void HCylinderParameterMenu::Spawn(float xOffset, float yOffset)
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - width_, yOffset), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width_, height_), ImGuiCond_FirstUseEver);

	ImGui::Begin("Hollowed cylinder parameters");
	ImGui::Text("Offsets (mm):");
	ImGui::InputText("X", xbuf_, IM_ARRAYSIZE(xbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Y", ybuf_, IM_ARRAYSIZE(ybuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Z", zbuf_, IM_ARRAYSIZE(zbuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Rotations (radians):");

	ImGui::SliderFloat("Theta", &theta_, 0.0f, 6.283185307f);
	ImGui::SliderFloat("Phi", &phi_, 0.0f, 6.283185307f);

	//Plane Specific Parameters

	ImGui::Text("Dimensions (mm):");
	ImGui::InputText("Inner Radius", radiusinnerbuf_, IM_ARRAYSIZE(radiusinnerbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Outer Radius", radiusouterbuf_, IM_ARRAYSIZE(radiusouterbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Height", heightbuf_, IM_ARRAYSIZE(heightbuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Voltage (V):");
	ImGui::InputText("Voltage", voltagebuf_, IM_ARRAYSIZE(voltagebuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Object name:");
	ImGui::InputText("Name", namebuf_, IM_ARRAYSIZE(namebuf_));

	ImGui::Checkbox("Attach empty object", &attachEmpty_);

	if (attachEmpty_)
	{
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(ImVec2(0, main_viewport->Size.y - height_), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(width_, height_), ImGuiCond_FirstUseEver);

		ImGui::Begin("Empty Cylinder parameters");
		ImGui::Text("Offsets (mm):");
		ImGui::InputText("X", xbufempty_, IM_ARRAYSIZE(xbufempty_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Y", ybufempty_, IM_ARRAYSIZE(ybufempty_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Z", zbufempty_, IM_ARRAYSIZE(zbufempty_), ImGuiInputTextFlags_CharsDecimal);

		ImGui::Text("Rotations (radians):");

		ImGui::SliderFloat("Theta", &thetaempty_, 0.0f, 6.283185307f);
		ImGui::SliderFloat("Phi", &phiempty_, 0.0f, 6.283185307f);

		ImGui::Text("Dimensions (mm):");
		ImGui::InputText("Radius", radiusbufempty_, IM_ARRAYSIZE(radiusbufempty_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Height", heightbufempty_, IM_ARRAYSIZE(heightbufempty_), ImGuiInputTextFlags_CharsDecimal);

		ImGui::End();
	}

	if (ImGui::Button("Place object"))
	{
		ObjectPendingCreation = true;
	}

	ImGui::End();
}

HCylinder HCylinderParameterMenu::GenerateObject()
{
	HCylinder cylinder;

	cylinder.x = (float)atof(xbuf_);
	cylinder.y = (float)atof(ybuf_);
	cylinder.z = (float)atof(zbuf_);
	cylinder.innerRadius = (float)atof(radiusinnerbuf_);
	cylinder.outerRadius = (float)atof(radiusouterbuf_);
	cylinder.height = (float)atof(heightbuf_);
	cylinder.voltage = (float)atof(voltagebuf_);
	cylinder.theta = theta_;
	cylinder.phi = phi_;
	std::copy(std::begin(namebuf_), std::end(namebuf_), std::begin(cylinder.name));

	if (attachEmpty_)
	{
		cylinder.emptyAttachment = true;
		Cylinder emptyCyl;

		emptyCyl.z = (float)atof(zbufempty_);
		emptyCyl.x = (float)atof(xbufempty_);
		emptyCyl.y = (float)atof(ybufempty_);
		emptyCyl.radius_ = (float)atof(radiusbufempty_);
		emptyCyl.height_ = (float)atof(heightbufempty_);
		emptyCyl.theta_ = thetaempty_;
		emptyCyl.phi_ = phiempty_;

		cylinder.empty = emptyCyl;
	}

	return cylinder;
}

void HCylinderParameterMenu::ResetObject()
{
	ObjectPendingCreation = false;
}
