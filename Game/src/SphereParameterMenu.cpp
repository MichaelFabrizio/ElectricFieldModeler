#include "SphereParameterMenu.h"
#include <imgui.h>
#include <iostream>
#include <cstdlib>

SphereParameterMenu::SphereParameterMenu() : width_(300), height_(400), xbuf_{ "" }, ybuf_{ "" }, zbuf_{ "" }, radiusbuf_{ "" },
ObjectPendingCreation(false), namebuf_{ "" }
{
};

void SphereParameterMenu::Spawn(float xOffset, float yOffset)
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - width_, yOffset), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width_, height_), ImGuiCond_FirstUseEver);

	ImGui::Begin("Sphere parameters");
	ImGui::Text("Offsets (mm):");
	ImGui::InputText("X", xbuf_, IM_ARRAYSIZE(xbuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Y", ybuf_, IM_ARRAYSIZE(ybuf_), ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputText("Z", zbuf_, IM_ARRAYSIZE(zbuf_), ImGuiInputTextFlags_CharsDecimal);

	//Plane Specific Parameters

	ImGui::Text("Dimensions (mm):");
	ImGui::InputText("Radius", radiusbuf_, IM_ARRAYSIZE(radiusbuf_), ImGuiInputTextFlags_CharsDecimal);

	ImGui::Text("Object name:");
	ImGui::InputText("Name", namebuf_, IM_ARRAYSIZE(namebuf_));

	if (ImGui::Button("Place object"))
	{
		ObjectPendingCreation = true;
	}

	ImGui::End();
}

Sphere SphereParameterMenu::GenerateObject()
{
	Sphere sphere;
	sphere.x = (float)atof(xbuf_);
	sphere.y = (float)atof(ybuf_);
	sphere.z = (float)atof(zbuf_);
	sphere.radius = (float)atof(radiusbuf_);
	std::copy(std::begin(namebuf_), std::end(namebuf_), std::begin(sphere.name));

	return sphere;
}

void SphereParameterMenu::ResetObject()
{
	ObjectPendingCreation = false;
}