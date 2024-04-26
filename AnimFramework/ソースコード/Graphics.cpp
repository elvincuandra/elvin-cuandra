#include "Graphics.h"

#define SAFE_RELEASE(p) { if(p) { p.Reset(); } }

ComPtr<ID3D11Device>			Graphics::m_pDevice{};
ComPtr<ID3D11DeviceContext>		Graphics::m_pDeviceContext{};
ComPtr<IDXGISwapChain>			Graphics::m_pSwapChain{};
ComPtr<ID3D11RenderTargetView>	Graphics::m_pRenderTargetView{};
ComPtr<ID3D11DepthStencilState> Graphics::m_pDepthStencilState{};
ComPtr<ID3D11DepthStencilView>	Graphics::m_pDepthStencilView{};
ComPtr<ID3D11RasterizerState>	Graphics::m_pRasterizerState{};
ComPtr<ID3D11SamplerState>		Graphics::m_pSamplerState{};
bool							Graphics::m_vsync{};
ComPtr<ID3D11Buffer>			Graphics::m_pWorldBuffer{};
ComPtr<ID3D11Buffer>			Graphics::m_pViewBuffer{};
ComPtr<ID3D11Buffer>			Graphics::m_pProjBuffer{};
DirectX::XMFLOAT4X4				Graphics::m_world{};
DirectX::XMFLOAT4X4				Graphics::m_view{};
DirectX::XMFLOAT4X4				Graphics::m_proj{};


HRESULT Graphics::Init(uint32_t width, uint32_t height, HWND hWnd, bool vsync, bool fullscreen)
{
	HRESULT hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC sd = {};
	ZeroMemory(&sd, sizeof(sd));

	m_vsync = vsync;

	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = fullscreen ? FALSE : TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		m_pSwapChain.GetAddressOf(),
		m_pDevice.GetAddressOf(),
		&featureLevel,
		m_pDeviceContext.GetAddressOf());

	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Failed to create Device and Swapchain", NULL, MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	// render target view
	ComPtr<ID3D11Texture2D> renderTarget;
	// get pointer to back buffer
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)renderTarget.GetAddressOf());
	// create the target view with back buffer
	hr = m_pDevice->CreateRenderTargetView(renderTarget.Get(), NULL, m_pRenderTargetView.GetAddressOf());
	renderTarget->Release();

	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Failed to create RenderTarget", NULL, MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	// depth/stencil buffer descriptor
	ComPtr<ID3D11Texture2D> depthStencilBuffer{};
	D3D11_TEXTURE2D_DESC depthBuffer{};
	ZeroMemory(&depthBuffer, sizeof(depthBuffer));

	depthBuffer.Width = width;
	depthBuffer.Height = height;
	depthBuffer.MipLevels = 1;
	depthBuffer.ArraySize = 1;
	depthBuffer.Format = DXGI_FORMAT_D32_FLOAT;
	depthBuffer.SampleDesc = sd.SampleDesc;
	depthBuffer.Usage = D3D11_USAGE_DEFAULT;
	depthBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBuffer.CPUAccessFlags = 0;
	depthBuffer.MiscFlags = 0;

	// create depth/stencil buffer
	hr = m_pDevice->CreateTexture2D(&depthBuffer, NULL, depthStencilBuffer.GetAddressOf());

	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Failed to create Depth/Stencil Buffer", NULL, MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	// depth/stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// depth/stencil descriptor
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, m_pDepthStencilState.GetAddressOf());

	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Failed to create Depth/Stencil State", NULL, MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);

	// depth/stencil view descriptor
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = depthBuffer.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, m_pDepthStencilView.GetAddressOf());
	depthStencilBuffer->Release();

	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Failed to create Depth/Stencil View", NULL, MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	// create rasterizer state
	hr = m_pDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerState.GetAddressOf());
	
	if (FAILED(hr)) {
		MessageBoxW(hWnd, L"Failed to create RasterizerState", NULL, MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	// set the rasterizer state.
	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	// Setup the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	// set the viewport
	m_pDeviceContext->RSSetViewports(1, &viewport);

	// create sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pDevice->CreateSamplerState(&samplerDesc, m_pSamplerState.GetAddressOf());
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

	// world view projection buffers
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_pDevice->CreateBuffer(&bufferDesc, NULL ,m_pWorldBuffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pWorldBuffer.GetAddressOf());

	m_pDevice->CreateBuffer(&bufferDesc, NULL, m_pViewBuffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());

	m_pDevice->CreateBuffer(&bufferDesc, NULL, m_pProjBuffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pProjBuffer.GetAddressOf());

	return hr;
}

void Graphics::Begin()
{
	float color[4] = { .5f, .5f, .5f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::End()
{
	if (m_vsync) {
		m_pSwapChain->Present(1, 0);
	}
	else {
		m_pSwapChain->Present(0, 0);
	}
}

void Graphics::Shutdown()
{
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

void Graphics::SetWorld(DirectX::XMMATRIX world)
{
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixTranspose(world));

	m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, NULL, &m_world, 0, 0);
}

void Graphics::SetView(DirectX::XMMATRIX view)
{
	DirectX::XMStoreFloat4x4(&m_view ,DirectX::XMMatrixTranspose(view));

	m_pDeviceContext->UpdateSubresource(m_pViewBuffer.Get(), 0, NULL, &m_view, 0, 0);

}

void Graphics::SetProj(DirectX::XMMATRIX proj)
{
	DirectX::XMStoreFloat4x4(&m_proj, DirectX::XMMatrixTranspose(proj));

	m_pDeviceContext->UpdateSubresource(m_pProjBuffer.Get(), 0, NULL, &m_proj, 0, 0);
}
