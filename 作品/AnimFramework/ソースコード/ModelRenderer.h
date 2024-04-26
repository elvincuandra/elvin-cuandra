#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#include "ModelLoader.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>

/** @struct BoneCombMatrix
 *  @brief This structure contains the bone combination matrix data to be sent to the GPU.
 *  @var VERTEX_3D::BoneCombMtx[400]
 *  Member 'BoneCombMtx' is an array of bone combination matrices.
 */
struct BoneCombMatrix {
	DirectX::XMFLOAT4X4 BoneCombMtx[400];
};

//! ModelRenderer class
/** This class is responsible for rendering the 3D model.
 *  It uses the ModelLoader class to access the model data. 
 */
class ModelRenderer
{
public:
	//! Init function<br>
	/** This function initializes the model renderer with data from mesh.
	 *  @param mesh : pointer to the mesh
	 */ 
	void Init(std::unique_ptr<ModelLoader>& mesh);

	//! Update function<br>
	/** This function updates the BoneCombMtx data.
	 *  @param mesh : pointer to the mesh
	 */
	void Update(std::unique_ptr<ModelLoader>& mesh);

	//! Draw function<br>
	/** This function renders the model.
	 */
	void Draw();

	//! Uninit function<br>
	/** This function terminates the model renderer.
	 *  and releases any remaining pointers
	 */
	void Uninit();
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBoneCombMtxBuffer; //!< This variable stores the bone combination matrix constant buffer.
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pDiffuseSRV; //!< This variable stores the shader resource view.

	std::vector<SUBSET> m_Subsets; //!< This variable stores the subsets data of a mesh.
	std::vector<CBMaterial*> m_Materials; //! < Constant Buffer for Material

	VertexBuffer<VERTEX_3D> m_VertexBuffer; //!< This variable stores the vertex buffer.
	IndexBuffer m_IndexBuffer; //!< This variable stores the index buffer.

public:
#pragma region Getter
	/** returns a pointer to the bone combination matrix constant buffer*/
	ID3D11Buffer* GetBoneMtxBuffer() const { return m_pBoneCombMtxBuffer.Get(); }
	/** returns a vector of shader resource views*/
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> GetSRV() const { return m_pDiffuseSRV; }
	/** returns a vector of subsets*/
	std::vector<CBMaterial*> GetMaterials() const { return m_Materials; }
	/** returns a vector of subsets*/
	VertexBuffer<VERTEX_3D> GetVertexBuffer() const { return m_VertexBuffer; }
	/** returns a pointer to the index buffer*/
	IndexBuffer GetIndexBuffer() const { return m_IndexBuffer; }
#pragma endregion

};

#endif // MODELRENDERER_H