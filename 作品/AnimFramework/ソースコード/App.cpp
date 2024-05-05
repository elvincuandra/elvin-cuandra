#include "App.h"
#include "fpscontrol.h"
#include "Manager.h"
#include "ImGuiManager.h"

const auto className = TEXT("Class Name");
const auto windowName = TEXT("Window Name");

App::App(uint32_t width, uint32_t height) 
	: 
	m_width(width),
	m_height(height),
	m_hWnd(nullptr),
	m_hInst(nullptr)
{
}

App::~App()
{
}


void App::Run()
{
	if (InitApp()) {
		MainLoop();
	}

	TermApp();
}

bool App::InitWnd()
{
	auto hInst = GetModuleHandle( nullptr);
	if (hInst == nullptr) {
		return false; 
	} 
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof( WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon( hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor( hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush( COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr; 
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon( hInst, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)){
		return false; 
	} 
	
	m_hInst = hInst;

	RECT rc = {}; 
	rc. right = static_cast <LONG>( m_width); 
	rc. bottom = static_cast <LONG>( m_height);
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(& rc, style, FALSE);
	m_hWnd = CreateWindowEx( 0,
		className, 
		windowName, 
		style, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		rc. right - rc. left,
		rc. bottom - rc. top, 
		nullptr, 
		nullptr,
		m_hInst, 
		nullptr);
	
	if (m_hWnd == nullptr) {
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);

	SetFocus(m_hWnd);

	return true;
}

void App::TermWnd()
{
	if (m_hWnd != nullptr) {
		DestroyWindow(m_hWnd);
	}

	if (m_hInst != nullptr) {
		UnregisterClass(className, m_hInst);
	}
	
	m_hWnd = nullptr;
	m_hInst = nullptr;
}

bool App::InitApp()
{
	if (!InitWnd()) {
		return false;
	}

	return true;
}

void App::TermApp()
{
	TermWnd();
}

void App::MainLoop()
{
	FPS fps;

	Manager::Init(m_width, m_height, m_hWnd);

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			auto deltaTime = fps.CalcDelta();
			Manager::Update(deltaTime);
			Manager::Draw(deltaTime);
		}
	}

	Manager::Uninit();
}

LRESULT App::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	switch (msg) {
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
		{
			// Do Nothing
			break;
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}