#include "Editor.h"
#include "PlayerModel.h"
#include "Camera.h"

void Editor::Init()
{
	auto camera = CreateObj<Camera>("Camera");
	auto playerModel = CreateObj<PlayerModel>("PlayerModel");
}

void Editor::Update(uint64_t deltaTime)
{
}

void Editor::Draw(uint64_t deltaTime)
{
   	auto camera = GetObj<Camera>("Camera");
	auto playerModel = GetObj<PlayerModel>("PlayerModel");

	camera->ShowCameraStatistics();
	playerModel->ShowMeshStatistics();
}

void Editor::Uninit()
{
}
