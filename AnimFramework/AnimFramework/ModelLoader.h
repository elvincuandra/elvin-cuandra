#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <unordered_map>

#include "Graphics.h"
#include "structs.h"

#include <directxtk/WICTextureLoader.h>
#include <directxtex/DirectXTex.h>

#include "AnimationLoader.h"
#include <memory>

#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTex.lib")


#if _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif

//! ModelLoader class
/** This class is responsible for loading model data from file and storing it in a container.
 *  It uses Assimp library to load model data from file. 
 */
class ModelLoader
{
public:
	//! LoadModel function<br>
	/** This function loads and parses model data from file.
	 *  @param fileName : file name of the model data
	 *  @return true if the model data is loaded successfully, false otherwise
	 */
	bool LoadModel(const std::string& fileName);

	//! Update function<br>
	/** This function updates the model data.
	 *  And is responsible for updating the bone matrix during an animation.
	 *  @param animData : pointer to the animation data
	 *  @param animName : name of the animation data
	 *  @param Frame1 : frame number
	 */
	void Update(std::unique_ptr<AnimationLoader>& animData, const char* animName, int Frame1);

private:
	//! CreateBone function<br>
	/** This function creates bone data from a parent node.
	 *  It is also a recursive function that calls itself to create bone data from child nodes.
	 *  The parsed bone data is then stored in container m_Bones.
	 *  @param node : pointer to the node
	 */
	void CreateBone(const aiNode* node);

	//! GetBoneInfo function<br>
	/** This function gets bone information from a mesh.
	 *  It includes the following : Bone Name, Mesh Name, Armature Name, the offset matrices and weight data
	 *  @param mesh : pointer to the mesh
	 *  @return vector of bone data
	 */
	std::vector<BONE> GetBoneInfo(const aiMesh* mesh);

	//! SetBoneDataToVertices function<br>
	/** This function sets bone data to vertices.
	 *  @param vertices : vector of vertices
	 *  @param subset : vector of subsets
	 *  @param subsetBone : vector of bone data
	 */
	void SetBoneDataToVertices(std::vector<VERTEX_3D>& vertices, std::vector<SUBSET>& subset, std::vector<std::vector<BONE>>& subsetBone);

	//! UpdateBoneMatrix function<br>
	/** This function updates the bone matrix.
	 *  @param node : pointer to the node
	 *  @param matrix : bone matrix
	 */
	void UpdateBoneMatrix(const aiNode* node, DirectX::XMMATRIX matrix);

	//! GetVerticesFromMesh function<br>
	/** This function gets vertices from a mesh.
	 *  @param mesh : pointer to the mesh
	 *  @return number of vertices
	 */
	unsigned int GetVerticesFromMesh(const aiMesh* mesh);

	//! GetIndicesFromMesh function<br>
	/** This function gets indices from a mesh.
	 *  @param mesh : pointer to the mesh
	 *  @return number of indices
	 */
	unsigned int GetIndicesFromMesh(const aiMesh* mesh);

	//! CalculateMeshBaseIndex function<br>
	/** This function calculates the base index of each mesh a scene has.
	 *  Responsible for calculating the base index the the next mesh.
	 *  @param subsets : vector of subsets
	 */
	void CalculateMeshBaseIndex(std::vector<SUBSET>& subsets);

	//! IsEmbeddedTexture function<br>
	/** This function checks if the texture is embedded.
	 *  @param texture : pointer to the texture
	 *  @return true if the texture is embedded, false otherwise
	 */
	bool IsEmbeddedTexture(const aiTexture* texture);

	//! GetTextures function<br>
	/** This function gets textures from a mesh.
	 *  @param mesh : pointer to the mesh
	 *  @return true if the texture is loaded successfully, false otherwise
	 */
	bool GetTextures(const aiScene* mesh);

	//! GetMaterials function<br>
	/** This function gets materials from a scene.
	 *  @param scene : pointer to the scene
	 */
	void GetMaterials(const aiScene* scene);

	//! GetMaterial function<br>
	/** This function converts aiMaterial to my own custom struct Material.
	 *  @param material : pointer to the material
	 */
	void GetMaterial(const aiMaterial* material);

	//! LoadDiffuseTexture function<br>
	/** This function loads the diffuse texture from file.
	 *  @param filename : file name of the texture
	 *  @return pointer to the shader resource view
	 */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadDiffuseTexture(const std::string filename);
private:
	std::unordered_map<std::string, BONE> m_Bones; //!< Bone container
	std::vector<SUBSET> m_Subsets; //!< Subset container
	std::vector<std::vector<BONE>> m_SubsetBones; //!< Subset bone container
	std::vector<VERTEX_3D> m_Vertices; //!< Vertex container
	std::vector<unsigned int> m_Indices; //!< Index container
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_Textures; //!< Texture container
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_AllTextures; //!< All texture container
	std::vector<MATERIAL> m_Materials; //!< Material container
	const aiScene* m_pScene; //!< Pointer to the scene

	std::vector<DirectX::XMMATRIX> m_BoneCombMtxContainer; //!< Bone matrix container

	std::string m_FilePath; //!< File path of the model data

public:
#pragma region Getter
	std::vector<VERTEX_3D> GetVertices() const { return m_Vertices; }
	std::vector<unsigned int> GetIndices() const { return m_Indices; }
	std::vector<MATERIAL> GetMaterials() const { return m_Materials; }
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> GetSRV() const { return m_AllTextures; }
	std::vector<SUBSET> GetSubsets() const { return m_Subsets; }
	std::vector<DirectX::XMMATRIX> GetBoneMatrix() const { return m_BoneCombMtxContainer; }
	std::unordered_map<std::string, BONE> GetBones() const { return m_Bones; }
#pragma endregion

#pragma region NotWorking
	// Setter
	void SetTranslation(std::string name, DirectX::XMFLOAT3 translation) { // Set translation offset to existing bone
		DirectX::XMFLOAT4X4 destination;
		DirectX::XMStoreFloat4x4(&destination, m_Bones[name].Matrix);
		destination._41 += translation.x;
		destination._42 += translation.y;
		destination._43 += translation.z;
		m_Bones[name].Matrix = DirectX::XMLoadFloat4x4(&destination);
		m_BoneCombMtxContainer[m_Bones[name].idx] = m_Bones[name].Matrix;

		DirectX::XMMATRIX rootMtx = DirectX::XMMatrixScaling(1, 1, 1) * DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(0, 0, 0, 1)) * DirectX::XMMatrixTranslation(0, 0, 0);
		UpdateBoneMatrix(m_pScene->mRootNode, rootMtx);
	}
#pragma endregion
};

#endif // MODELLOADER_H