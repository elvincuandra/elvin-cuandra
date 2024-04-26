#include "Shader.h"

void Shader::SetShader(std::string vs, std::string ps)
{
	HRESULT hr = E_FAIL;

	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDEX",	0,	DXGI_FORMAT_R32G32B32A32_SINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHT",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	unsigned int numElements = ARRAYSIZE(ied);

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = Graphics::GetDevice();

	hr = DirectXHelper::CreateVertexShader(
		pDevice.Get(),
		vs.c_str(),
		"main",
		"vs_5_0",
		ied,
		numElements,
		m_pVertexShader.GetAddressOf(),
		m_pInputLayout.GetAddressOf()
	);

	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed to create vertex shader", L"Error", MB_OK);
		return;
	}

	hr = DirectXHelper::CreatePixelShader(
		pDevice.Get(),
		ps.c_str(),
		"main",
		"ps_5_0",
		m_pPixelShader.GetAddressOf()
	);

	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed to create pixel shader", L"Error", MB_OK);
		return;
	}
}

void Shader::SetGPU()
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();

	pContext->IASetInputLayout(m_pInputLayout.Get());
	pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}
