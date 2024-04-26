#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H

#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class ImGuiManager
{
public:
	static void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void PreRender();
	static void Render();
	static void Shutdown();
};

#endif // IMGUIMANAGER_H
