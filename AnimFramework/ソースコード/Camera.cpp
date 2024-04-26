#include "Camera.h"
#include "dx11helper.h"
#include "Graphics.h"
#include <iostream>
#include "Input.h"
#include "Graphics.h"

constexpr float cameraMoveSpeed = 0.1f;

void Camera::Init()
{
	HRESULT hr = E_FAIL;

	m_position = DirectX::XMFLOAT3(0.0f, 0.0f, -100.0f);
	m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

}

void Camera::Update(uint64_t deltaTime)
{
	HRESULT hr = E_FAIL;

	if (Input::IsKeyPress('W')) {
		m_position.z += cameraMoveSpeed * deltaTime;
	}
	if (Input::IsKeyPress('S')) {
		m_position.z -= cameraMoveSpeed * deltaTime;
	}
	if (Input::IsKeyPress('A')) {
		m_position.x -= cameraMoveSpeed * deltaTime;
	}
	if (Input::IsKeyPress('D')) {
		m_position.x += cameraMoveSpeed * deltaTime;
	}
	if (Input::IsKeyPress('Q')) {
		m_position.y -= cameraMoveSpeed * deltaTime;
	}
	if (Input::IsKeyPress('E')) {
		m_position.y += cameraMoveSpeed * deltaTime;
	}
		
}

void Camera::Draw(uint64_t deltaTime)
{
	DirectX::XMVECTOR lookatTarget = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	auto currentPos = DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.f);
	DirectX::XMVECTOR lookAt = DirectX::XMVectorAdd(currentPos, lookatTarget);

	m_view = DirectX::XMMatrixLookAtLH(GetPositionXM(), lookAt, m_up);

	Graphics::SetView(m_view);
	
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);
	constexpr float aspectRatio = 16.f / 9.f;
	constexpr float NearZ = 0.1f;
	constexpr float FarZ = 1000.f;

	m_projection = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, NearZ, FarZ);

	Graphics::SetProj(m_projection);
}

void Camera::Uninit()
{
}

void Camera::ShowCameraStatistics()
{
	if (ImGui::Begin("Camera Statistics")) {
		ImGui::Text("Current Position : %f, %f, %f", m_position.x, m_position.y, m_position.z);

	}

	ImGui::End();
}

