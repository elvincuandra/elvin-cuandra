#pragma once

#include <string>
#include <DirectXMath.h>
#include <vector>

/** @struct VERTEX_3D
 *  @brief This structure contains the per-vertex data of a 3D model.
 *  @var VERTEX_3D::pos
 *  Member 'pos' contains the position of the vertex.
 *  @var VERTEX_3D::normal
 *  Member 'normal' contains the normal of the vertex.
 *  @var VERTEX_3D::diffuse
 *  Member 'diffuse' contains the diffuse color of the vertex.
 *  @var VERTEX_3D::uv
 *  Member 'uv' contains the texture coordinates of the vertex.
 *  @var VERTEX_3D::BoneIdx
 *  Member 'BoneIdx' contains the index of the bones that affect the vertex.
 *  @var VERTEX_3D::BoneWeight
 *  Member 'BoneWeight' contains the weight of the bones that affect the vertex.
 *  @var VERTEX_3D::BoneCount
 *  Member 'BoneCount' contains the number of bones that affect the vertex.
 */
struct VERTEX_3D {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT2 uv;
	int BoneIdx[4];
	float BoneWeight[4];
	int BoneCount = 0;
};

/** @struct SUBSET
 *  @brief This structure contains the subset data of a 3D model (mesh data).
 *  @var SUBSET::Name
 *  Member 'Name' contains the name of the subset.
 *  @var SUBSET::IndexNum
 *  Member 'IndexNum' contains the number of indices in the subset.
 *  @var SUBSET::VertexNum
 *  Member 'VertexNum' contains the number of vertices in the subset.
 *  @var SUBSET::IndexBase
 *  Member 'IndexBase' contains the base index of the subset.
 *  @var SUBSET::VertexBase
 *  Member 'VertexBase' contains the base vertex of the subset.
 *  @var SUBSET::MaterialIdx
 *  Member 'MaterialIdx' contains the index of the material that the subset uses.
 */
struct SUBSET {
	std::string		Name;
	unsigned int	IndexNum = 0;
	unsigned int	VertexNum = 0;
	unsigned int	IndexBase = 0;
	unsigned int	VertexBase = 0;
	unsigned int	MaterialIdx = 0;
};

/** @struct MATERIAL
 *  @brief This structure contains the material data of a 3D model.
 *  @var MATERIAL::Ambient
 *  Member 'Ambient' contains the ambient color of the material.
 *  @var MATERIAL::Diffuse
 *  Member 'Diffuse' contains the diffuse color of the material.
 *  @var MATERIAL::Specular
 *  Member 'Specular' contains the specular color of the material.
 *  @var MATERIAL::Emission
 *  Member 'Emission' contains the emission color of the material.
 *  @var MATERIAL::Shininess
 *  Member 'Shininess' contains the shininess of the material.
 *  @var MATERIAL::TextureEnable
 *  Member 'TextureEnable' contains the flag to enable texture.
 *  @var MATERIAL::Dummy
 *  Member 'Dummy' contains the dummy data.
 */
struct MATERIAL {
	DirectX::XMFLOAT4	Ambient;
	DirectX::XMFLOAT4	Diffuse;
	DirectX::XMFLOAT4	Specular;
	DirectX::XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float 	Dummy[2];
};

/** @struct WEIGHT
 *  @brief This structure contains the weight data of a 3D model.
 *  @var WEIGHT::BoneName
 *  Member 'BoneName' contains the name of the bone.
 *  @var WEIGHT::MeshName
 *  Member 'MeshName' contains the name of the mesh.
 *  @var WEIGHT::weight
 *  Member 'weight' contains the weight of the bone.
 *  @var WEIGHT::vertexIdx
 *  Member 'vertexIdx' contains the index of the vertex.
 */
struct WEIGHT {
	std::string BoneName;
	std::string MeshName;
	float weight;
	int vertexIdx;
};

/** @struct BONE
 *  @brief This structure contains the bone data of a 3D model.
 *  @var BONE::BoneName
 *  Member 'BoneName' contains the name of the bone.
 *  @var BONE::MeshName
 *  Member 'MeshName' contains the name of the mesh.
 *  @var BONE::ArmatureName
 *  Member 'ArmatureName' contains the name of the armature.
 *  @var BONE::idx
 *  Member 'idx' contains the index of the bone.
 *  @var BONE::Matrix
 *  Member 'Matrix' contains the matrix of the bone.
 *  @var BONE::OffsetMatrix
 *  Member 'OffsetMatrix' contains the offset matrix of the bone.
 *  @var BONE::AnimationMatrix
 *  Member 'AnimationMatrix' contains the animation matrix of the bone.
 *  @var BONE::Blend1
 *  Member 'Blend1' contains the blend matrix 1 of the bone.
 *  @var BONE::Blend2
 *  Member 'Blend2' contains the blend matrix 2 of the bone.
 *  @var BONE::weights
 *  Member 'weights' contains the weights of the bone.
 */
struct BONE {
	std::string BoneName;
	std::string MeshName;
	std::string ArmatureName;
	int idx = 0;
	DirectX::XMMATRIX Matrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX OffsetMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX AnimationMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX Blend1 = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX Blend2 = DirectX::XMMatrixIdentity();
	std::vector<WEIGHT> weights;
};
