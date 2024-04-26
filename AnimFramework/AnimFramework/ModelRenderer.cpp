#include "ModelRenderer.h"

void ModelRenderer::Init(std::unique_ptr<ModelLoader>& mesh)
{
	HRESULT hr = E_FAIL;


	m_VertexBuffer.Create(mesh->GetVertices());
	m_IndexBuffer.Create(mesh->GetIndices());

	const std::vector<MATERIAL>& materials = mesh->GetMaterials();

	for (auto& mtl : materials) {
		CBMaterial* m =  new CBMaterial;
		m->Init(mtl);
		m_Materials.emplace_back(m);
	}

	m_pDiffuseSRV = mesh->GetSRV();

	m_Subsets = mesh->GetSubsets();

	hr = DirectXHelper::CreateConstantBufferWrite(
		Graphics::GetDevice(),
		sizeof(BoneCombMatrix),
		m_pBoneCombMtxBuffer.GetAddressOf()
	);

	assert(hr == S_OK);
}

void ModelRenderer::Update(std::unique_ptr<ModelLoader>& mesh)
{
	const std::vector<DirectX::XMMATRIX> mtxContainer = mesh->GetBoneMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	BoneCombMatrix* pData = nullptr;

	HRESULT hr = Graphics::GetDeviceContext()->Map(
		m_pBoneCombMtxBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);

	if (SUCCEEDED(hr)) {
		pData = (BoneCombMatrix*)mappedResource.pData;
		memcpy(pData->BoneCombMtx,
			(void*)mtxContainer.data(),
			sizeof(DirectX::XMMATRIX) * mtxContainer.size()
		);

		Graphics::GetDeviceContext()->Unmap(m_pBoneCombMtxBuffer.Get(), 0);
	}
}

void ModelRenderer::Draw()
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();

	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->VSSetConstantBuffers(4, 1, m_pBoneCombMtxBuffer.GetAddressOf());

	for (auto& subset : m_Subsets) {
		pContext->PSSetShaderResources(0, 1, m_pDiffuseSRV[subset.MaterialIdx].GetAddressOf());

		m_Materials[subset.MaterialIdx]->SetGPU();

		pContext->DrawIndexed(
			subset.IndexNum,
			subset.IndexBase,
			subset.VertexBase
		);
	}
}

void ModelRenderer::Uninit()
{
	for (auto& mtl : m_Materials) {
		mtl->Uninit();
		delete mtl;
	}
}
