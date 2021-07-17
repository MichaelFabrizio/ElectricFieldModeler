#include "RPrismParameterMenu.h"
#include "Shapes/RPrism.h"
#include <imgui.h>
#include <iostream>

RPrismParameterMenu::RPrismParameterMenu() : width_(300), height_(400), xbuf_{ "" }, ybuf_{ "" }, zbuf_{ "" }, theta_(0), phi_(0),
lengthbuf_{ "" }, widthbuf_{ "" }, heightbuf_{ "" }, ObjectPendingCreation(false), namebuf_{ "" }, voltagebuf_{ "" }
{
};

void RPrismParameterMenu::Spawn(float xOffset, float yOffset)
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - width_, yOffset), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width_, height_), ImGuiCond_FirstUseEver);

	ImGui::Begin("Rectangular prism parameters");
	ImGui::Text("Offsets (mm):");
	ImGui::InputText("X", xbuf_, IM_ARRAYSIZE(xbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Y", ybuf_, IM_ARRAYSIZE(ybuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Z", zbuf_, IM_ARRAYSIZE(zbuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Rotations (radians):");

	ImGui::SliderFloat("Theta", &theta_, 0.0f, 6.283185307f);
	ImGui::SliderFloat("Phi", &phi_, 0.0f, 6.283185307f);

	//RPrism Specific Parameters

	ImGui::Text("Dimensions (mm):");
	ImGui::InputText("Length", lengthbuf_, IM_ARRAYSIZE(lengthbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Width", widthbuf_, IM_ARRAYSIZE(widthbuf_), ImGuiInputTextFlags_CharsDecimal);
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

RPrism RPrismParameterMenu::GenerateObject()
{
	RPrism rprism;

	rprism.x = (float)atof(xbuf_);
	rprism.y = (float)atof(ybuf_);
	rprism.z = (float)atof(zbuf_);
	rprism.length = (float)atof(lengthbuf_);
	rprism.width = (float)atof(widthbuf_);
	rprism.height = (float)atof(heightbuf_);
	rprism.voltage = (float)atof(voltagebuf_);
	rprism.theta = theta_;
	rprism.phi = phi_;
	std::copy(std::begin(namebuf_), std::end(namebuf_), std::begin(rprism.name));

	return rprism;
}

void RPrismParameterMenu::ResetObject()
{
	ObjectPendingCreation = false;
}