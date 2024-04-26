#ifndef MATERIAL_H
#define MATERIAL_H

#include "structs.h"
#include "Graphics.h"
#include "utility.h"
#include "dx11helper.h"
#include <iostream>

class CBMaterial
{
private:
	//! Constant buffer struct for material
	struct CBufferMaterial {
		MATERIAL Material;
	};

public:
	//! Init function<br>
	/** This function initializes the constant buffer for material.
	 *  @param mtl : material data
	 *  @return bool : returns true if successful
	 */
	bool Init(MATERIAL mtl) {
		HRESULT hr = E_FAIL;

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = Graphics::GetDevice();

		hr = DirectXHelper::CreateConstantBuffer(pDevice.Get(), sizeof(CBufferMaterial), m_pConstantBufferMaterial.GetAddressOf());

		if (FAILED(hr)) {
			std::cout << "Failed to create constant buffer for material" << std::endl;
			return false;
		}

		// Store mtl to member variable m_Material
		m_Material = mtl;

		Update();

		return true;
	};

	//! Update function<br>
	/** This function updates the constant buffer for material and sets it to a constant buffer slot.
	 */
	void Update() {
		CBufferMaterial cb;
		cb.Material = m_Material;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();
		pContext->UpdateSubresource(
			m_pConstantBufferMaterial.Get(),
			0,
			nullptr,
			&cb,
			0,
			0);

		pContext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
		pContext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
	};

	//! SetGPU function<br>
	/** Set the constant buffer to a slot*/
	void SetGPU() {
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();

		pContext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
		pContext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
	};

	//! SetMaterial function<br>
	/** This function sets the material data and updates the constant buffer for material.
	 *  @param mtl : material data
	 */
	void SetMaterial(const MATERIAL& mtl) {
		CBufferMaterial cbm;
		cbm.Material = mtl;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();

		pContext->UpdateSubresource(
			m_pConstantBufferMaterial.Get(),
			0,
			nullptr,
			&cbm,
			0,
			0);

		pContext->VSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
		pContext->PSSetConstantBuffers(3, 1, m_pConstantBufferMaterial.GetAddressOf());
	};

	//! Uninit function<br>
	//! This function releases the constant buffer for material.
	void Uninit() {
		if (m_pConstantBufferMaterial) {
			m_pConstantBufferMaterial.Reset();
		}
	};


	//! SetDiffuse function<br>
	//! This function sets the diffuse color of the material.
	void SetDiffuse(DirectX::XMFLOAT4 diffuse) {
		m_Material.Diffuse = diffuse;
	};

	//! SetAmbient function<br>
	//! This function sets the ambient color of the material.
	void SetAmbient(DirectX::XMFLOAT4 ambient) {
		m_Material.Ambient = ambient;
	};

	//! SetSpecular function<br>
	//! This function sets the specular color of the material.
	void SetSpecular(DirectX::XMFLOAT4 specular) {
		m_Material.Specular = specular;
	};

	//! SetEmission function<br>
	//! This function sets the emission color of the material.
	void SetEmission(DirectX::XMFLOAT4 emission) {
		m_Material.Emission = emission;
	};

	//! SetShininess function<br>
	//! This function sets the shininess of the material.
	void SetShininess(float shininess) {
		m_Material.Shininess = shininess;
	};

	MATERIAL m_Material; //!< This variable stores the material data.
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferMaterial; //!< This variable stores the constant buffer for material.

public:
	//! GetMaterial function<br>
	/** This function returns the material data.
	 *   @return MATERIAL : material data
	 */
	MATERIAL GetMaterial() const { return m_Material; }

	//! GetConstantBuffer function<br>
	/** This function returns the constant buffer for material.
	 *  @return ID3D11Buffer* : pointer to the constant buffer for material
	 */
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetConstantBuffer() const { return m_pConstantBufferMaterial; }
};

#endif // MATERIAL_H