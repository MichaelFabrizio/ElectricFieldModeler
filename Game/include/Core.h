#pragma once
#include "Renderer.h"
#include "ContentManager.h"
#include "Shapes/Shape.h"
#include "Laplacian.h"
#include "SerializationManager.h"
#include <SDL.h>
#include <imgui.h>
#include <vector>
#include <memory>

class Core
{
public:
	Core();
	~Core();

	void Init();
	void Run();
	void Clean();
	void TransferObjectList(std::vector<std::unique_ptr<Shape>>& pendingAddition);

	Renderer* renderer;
	ContentManager* contentManager_;
private:
	bool _isRunning;
	SDL_Window* _window;
	SDL_GLContext _glContext;
	ImGuiIO* io_;
	std::vector<std::unique_ptr<Shape>> objectList_;
	std::vector<std::unique_ptr<Shape>> listToAdd_;
	std::vector<int> pendingDeletion_;
	bool calculateClicked_;
	Laplacian* laplacian;
	SerializationManager* serializer_;

	char gridxbuf_[16];
	char gridybuf_[16];
	char gridzbuf_[16];
	char gridscalebuf_[16];
	char iterationbuf_[16];
};