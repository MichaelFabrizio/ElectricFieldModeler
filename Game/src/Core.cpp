#include "Core.h"
#include "Log.h"
#include "ObjectSelectionMenu.h"
#include "MCIntegrator.h"
#include "Laplacian.h"

#include <SDL.h>
#include <Glad.h>
#include <imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <thread>

Core::Core() : _isRunning(true), _window(nullptr), _glContext(NULL), renderer(nullptr), contentManager_(nullptr), io_(nullptr),
calculateClicked_(false), laplacian(nullptr), gridxbuf_{ "" }, gridybuf_{ "" }, gridzbuf_{ "" }, gridscalebuf_{ "" }, iterationbuf_{ "" }
{
	serializer_ = new SerializationManager();
}

Core::~Core()
{
	delete renderer;
	delete serializer_;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(_glContext);
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}
//TODO: Add quit behavior to ASSERT macro
void Core::Init()
{
	ASSERT(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0, "SDL failed to initialize");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	const char* glsl_version = "#version 330 core";

	_window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

	ASSERT(!_window, "Window creation failed");

	_glContext = SDL_GL_CreateContext(_window);

	ASSERT(!_glContext, "SDL-OpenGL context creation failed");

	ASSERT(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Glad loader initialization failed");

	//IMGUI state functions and spin-up for SDL + OpenGL
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io_ = &ImGui::GetIO(); (void)io_;
	//Disables automatic window config
	io_->IniFilename = NULL;

	ImGui::StyleColorsClassic();

	ImGui_ImplSDL2_InitForOpenGL(_window, _glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);



	//Should organize lesser "state" settings, more will pop up
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(_window);

}

void Core::Run()
{
	bool show_demo_window = false;
	ObjectSelectionMenu menu;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	while (_isRunning)
	{
		//Created on the stack I think
		SDL_Event evnt;

		while (SDL_PollEvent(&evnt))
		{
			ImGui_ImplSDL2_ProcessEvent(&evnt);
			switch (evnt.type)
			{
			case SDL_QUIT:
				//SDL_Quit(); Causes bugs with ImGui if SDL is quit this early in the loop
				_isRunning = false;
				break;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window);
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		menu.GenerateWindow();
		menu.Process();
		menu.EmptyList(listToAdd_);
		objectList_.insert(objectList_.end(), std::make_move_iterator(listToAdd_.begin()), std::make_move_iterator(listToAdd_.end()));

		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - 300-300, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Object List");
		for (size_t i = 0; i < objectList_.size(); i++)
		{
			ImGui::Text((*objectList_[i]).name); ImGui::SameLine();
			ImGui::SmallButton("Select"); ImGui::SameLine();
			if (ImGui::SmallButton("Delete"))
			{
				pendingDeletion_.push_back(i);
			}
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - 300 - 100, 200), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);

		int windowflags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground;

		ImGui::Begin("Calculate", NULL, windowflags);
		if (ImGui::Button("Calculate"))
		{
			laplacian = new Laplacian(atoi(iterationbuf_), atoi(gridxbuf_), atoi(gridybuf_), atoi(gridzbuf_), (float)atof(gridscalebuf_));
			laplacian->Calculate(objectList_);
			//laplacian->DebugPrint();
			laplacian->ExportCSV();
			delete laplacian;
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(main_viewport->Size.x - 300 - 300 - 350, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Grid Parameters");
		ImGui::InputText("Iterations", iterationbuf_, IM_ARRAYSIZE(iterationbuf_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Grid X Length", gridxbuf_, IM_ARRAYSIZE(gridxbuf_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Grid Y Length", gridybuf_, IM_ARRAYSIZE(gridybuf_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Grid Z Length", gridzbuf_, IM_ARRAYSIZE(gridzbuf_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("Grid Scaling", gridscalebuf_, IM_ARRAYSIZE(gridscalebuf_), ImGuiInputTextFlags_CharsDecimal);
		ImGui::End();

		serializer_->BuildWindow(*main_viewport, objectList_);

		// Rendering
		ImGui::Render();

		glViewport(0, 0, (int)io_->DisplaySize.x, (int)io_->DisplaySize.y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(_window);

		Clean();
	}
}

void Core::Clean()
{
	for (size_t i = 0; i < pendingDeletion_.size(); i++)
	{
		objectList_.erase(objectList_.begin() + pendingDeletion_[i]);
	}

	pendingDeletion_.clear();
}

void Core::TransferObjectList(std::vector<std::unique_ptr<Shape>>& pendingAddition)
{

}
