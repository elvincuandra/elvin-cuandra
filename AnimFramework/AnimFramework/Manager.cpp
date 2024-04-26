#include "Manager.h"
#include "Graphics.h"
#include "Editor.h"
#include "ImGuiManager.h"
#include "Input.h"

std::unique_ptr<Scene> Manager::m_pScene = nullptr;

void Manager::Init(uint32_t width, uint32_t height, HWND hWnd)
{
	Graphics::Init(width, height, hWnd);

	ImGuiManager::Init(hWnd, Graphics::GetDevice(), Graphics::GetDeviceContext());

	Input::Init();

	SetScene<Editor>();
}

void Manager::Update(uint64_t deltaTime)
{
	Input::Update();

	m_pScene->UpdateBase(deltaTime);
}

void Manager::Draw(uint64_t deltaTime)
{
	Graphics::Begin();

	ImGuiManager::PreRender();

	// Draw Here
	m_pScene->DrawBase(deltaTime);

	ImGuiManager::Render();

	Graphics::End();
}

void Manager::Uninit()
{
	m_pScene->UninitBase();

	ImGuiManager::Shutdown();

	Graphics::Shutdown();
}
