#include "ModelLoader.h"
#include <iostream>
#include "utility.h"

bool ModelLoader::LoadModel(const std::string& fileName)
{
	static Assimp::Importer importer;

	m_FilePath = fileName;

	const aiScene* pScene = importer.ReadFile(
		fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_ConvertToLeftHanded |
		aiProcess_PopulateArmatureData
	);

	if (!pScene) {
		std::cout << "Error parsing '" << fileName.c_str() << "': " << importer.GetErrorString() << std::endl;
		return false;
	}

	CreateBone(pScene->mRootNode);

	unsigned int num = 0;
	for (auto& data : m_Bones) {
		data.second.idx = num;
		num++;
	}

	m_Subsets.resize(pScene->mNumMeshes);

	for (unsigned int m = 0; m < pScene->mNumMeshes; m++) {
		aiMesh* pMesh;
		pMesh = pScene->mMeshes[m];

		int vertexNum = GetVerticesFromMesh(pMesh);

		int indexNum = GetIndicesFromMesh(pMesh);

		int materialIdx = pMesh->mMaterialIndex;

		m_Subsets[m].Name = std::string(pMesh->mName.C_Str());

		m_Subsets[m].IndexNum = indexNum;

		m_Subsets[m].VertexNum = vertexNum;

		m_Subsets[m].MaterialIdx = materialIdx;

		std::vector<BONE> subsetBone = GetBoneInfo(pMesh);

		if (subsetBone.size() > 0) {
			std::size_t s = subsetBone.size();
		}

		m_SubsetBones.emplace_back(subsetBone);
	}

	CalculateMeshBaseIndex(m_Subsets);

	SetBoneDataToVertices(m_Vertices, m_Subsets, m_SubsetBones);

	GetTextures(pScene);

	GetMaterials(pScene);

	m_pScene = pScene;

	return true;
}

void ModelLoader::Update(std::unique_ptr<AnimationLoader>& animData, const char* animName, int Frame1)
{
	DirectX::XMMATRIX rootMatrix;

	DirectX::XMFLOAT3 scale = {1.f, 1.f, 1.f};
	DirectX::XMFLOAT3 position = { 0, 0, 0};
	DirectX::XMFLOAT4 rotation;
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z = 0.f;
	rotation.w = 1.f;

	DirectX::XMMATRIX scaleMtx = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX rotMtx = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
	DirectX::XMMATRIX transMtx = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	rootMatrix = scaleMtx * rotMtx * transMtx;

	aiAnimation* animation1 = animData->GetAnimation(animName, 0);

	for (unsigned int c = 0; c < animation1->mNumChannels; c++) {
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bones[nodeAnim->mNodeName.C_Str()];

		int frame;

		frame = Frame1 % nodeAnim->mNumRotationKeys;
		aiQuaternion rot = nodeAnim->mRotationKeys[frame].mValue;

		frame = Frame1 % nodeAnim->mNumPositionKeys;
		aiVector3D pos = nodeAnim->mPositionKeys[frame].mValue;

		DirectX::XMFLOAT3 scale = { 1.f, 1.f, 1.f };
		DirectX::XMFLOAT3 position = { pos.x, pos.y, pos.z };
		DirectX::XMFLOAT4 rotation;
		rotation.x = rot.x;
		rotation.y = rot.y;
		rotation.z = rot.z;
		rotation.w = rot.w;

		DirectX::XMMATRIX scaleMtx = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX rotMtx = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
		DirectX::XMMATRIX transMtx = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		bone->AnimationMatrix = scaleMtx * rotMtx * transMtx;
	}

	UpdateBoneMatrix(m_pScene->mRootNode, rootMatrix);

	m_BoneCombMtxContainer.clear();
	m_BoneCombMtxContainer.resize(m_Bones.size());
	for (auto& data : m_Bones) {
		m_BoneCombMtxContainer[data.second.idx] = data.second.Matrix;
	}

}

void ModelLoader::CreateBone(const aiNode* node)
{
	BONE bone;

	m_Bones[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		CreateBone(node->mChildren[n]);
	}
}

std::vector<BONE> ModelLoader::GetBoneInfo(const aiMesh* mesh)
{
	std::vector<BONE> bones;

	for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {
		BONE bone;
		bone.BoneName = std::string(mesh->mBones[bidx]->mName.C_Str());

		bone.MeshName = std::string(mesh->mBones[bidx]->mNode->mName.C_Str());

		bone.ArmatureName = std::string(mesh->mBones[bidx]->mArmature->mName.C_Str());

		std::cout << bone.BoneName <<
			"(" << bone.MeshName << ")" <<
			"(" << bone.ArmatureName << ")" <<
			std::endl;

		bone.OffsetMatrix = utility::ConvertaiMtxTodxMtx(mesh->mBones[bidx]->mOffsetMatrix);

		bone.weights.clear();

		for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {
			WEIGHT w;

			w.MeshName = bone.MeshName;
			w.BoneName = bone.BoneName;

			w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;
			w.vertexIdx = mesh->mBones[bidx]->mWeights[widx].mVertexId;

			bone.weights.emplace_back(w);
		}

		bones.emplace_back(bone);

		m_Bones[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = utility::ConvertaiMtxTodxMtx(mesh->mBones[bidx]->mOffsetMatrix);
	}

	return bones;
}

void ModelLoader::SetBoneDataToVertices(
	std::vector<VERTEX_3D>& vertices,
	std::vector<SUBSET>& subset,
	std::vector<std::vector<BONE>>& subsetBone)
{
	// Initialize Bone Index
	for (auto& v : vertices) {
		v.BoneCount = 0;
		for (int b = 0; b < 4; b++) {
			v.BoneIdx[b] = -1;
			v.BoneWeight[b] = 0;
		}
	}

	int subsetID = 0;
	for (auto& bones : subsetBone) {
		int vertexBase = subset[subsetID].VertexBase;

		for (auto& bone : bones) {
			for (auto& w : bone.weights) {
				int& idx = vertices[vertexBase + w.vertexIdx].BoneCount;
				vertices[vertexBase + w.vertexIdx].BoneWeight[idx] = w.weight;
				vertices[vertexBase + w.vertexIdx].BoneIdx[idx] = m_Bones[w.BoneName].idx;
				idx++;
				assert(idx <= 4);
			}
		}

		subsetID++;
	}
}

void ModelLoader::UpdateBoneMatrix(const aiNode* node, DirectX::XMMATRIX matrix)
{
	BONE* bone = &m_Bones[node->mName.C_Str()];

	DirectX::XMMATRIX boneCombMtx;
	boneCombMtx = bone->OffsetMatrix * bone->AnimationMatrix * matrix;

	bone->Matrix = boneCombMtx;

	DirectX::XMMATRIX selfMatrix;
	selfMatrix = bone->AnimationMatrix * matrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		UpdateBoneMatrix(node->mChildren[n], selfMatrix);
	}
}


unsigned int ModelLoader::GetVerticesFromMesh(const aiMesh* mesh)
{
	for (unsigned int vidx = 0; vidx < mesh->mNumVertices; vidx++) {
		VERTEX_3D v;

		v.pos.x = mesh->mVertices[vidx].x;
		v.pos.y = mesh->mVertices[vidx].y;
		v.pos.z = mesh->mVertices[vidx].z;

		if (mesh->HasNormals()) {
			v.normal.x = mesh->mNormals[vidx].x;
			v.normal.y = mesh->mNormals[vidx].y;
			v.normal.z = mesh->mNormals[vidx].z;
		}
		else {
			v.normal = DirectX::XMFLOAT3(0, 0, 0);
		}

		if (mesh->HasTextureCoords(0)) {
			v.uv.x = mesh->mTextureCoords[0][vidx].x;
			v.uv.y = mesh->mTextureCoords[0][vidx].y;
		}
		else {
			v.uv = DirectX::XMFLOAT2(0, 0);
		}

		if (mesh->HasVertexColors(0)) {
			v.diffuse.x = mesh->mColors[0][vidx].r;
			v.diffuse.y = mesh->mColors[0][vidx].g;
			v.diffuse.z = mesh->mColors[0][vidx].b;
			v.diffuse.w = mesh->mColors[0][vidx].a;
		}
		else {
			v.diffuse = DirectX::XMFLOAT4(1, 1, 1, 1);
		}

		m_Vertices.emplace_back(v);
	}
	return mesh->mNumVertices;
}

unsigned int ModelLoader::GetIndicesFromMesh(const aiMesh* mesh)
{
	m_Indices.reserve(mesh->mNumFaces * 3);

	for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
		const aiFace& face = mesh->mFaces[f];
		assert(face.mNumIndices == 3);

		m_Indices.emplace_back(face.mIndices[0]);
		m_Indices.emplace_back(face.mIndices[1]);
		m_Indices.emplace_back(face.mIndices[2]);
	}

	return mesh->mNumFaces * 3;
}

void ModelLoader::CalculateMeshBaseIndex(std::vector<SUBSET>& subsets)
{
	for (int m = 0; m < subsets.size(); m++) {
		subsets[m].VertexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			subsets[m].VertexBase += subsets[i].VertexNum;
		}

		subsets[m].IndexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			subsets[m].IndexBase += subsets[i].IndexNum;
		}
	}
}

bool ModelLoader::IsEmbeddedTexture(const aiTexture* texture)
{
	if (texture->mHeight == 0 ||
		texture->mWidth == 0 ||
		texture->pcData == nullptr) {
		return true;
	}

	return false;
}

bool ModelLoader::GetTextures(const aiScene* scene)
{
	auto device = Graphics::GetDevice();
	auto deviceContext = Graphics::GetDeviceContext();

	for (unsigned int texID = 0; texID < scene->mNumTextures; texID++) {
		aiTexture* texture = scene->mTextures[texID];

		if (IsEmbeddedTexture(texture)) {
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

			const size_t size = texture->mWidth;

			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				device,
				deviceContext,
				reinterpret_cast<const unsigned char*>(texture->pcData),
				texture->mWidth,
				nullptr,
				srv.GetAddressOf()
			);

			if (FAILED(hr)) {
				assert(srv);
				return false;
			}

			m_Textures[texture->mFilename.data] = srv;
		}
		else {
			std::string filename = utility::GetFileName(texture->mFilename.C_Str());

			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;

			const wchar_t* ws;

			std::wstring wfilename = std::wstring(filename.begin(), filename.end());
			ws = wfilename.c_str();

			HRESULT hr = DirectX::CreateWICTextureFromFile(
				device,
				deviceContext,
				ws,
				resource.GetAddressOf(),
				srv.GetAddressOf()
			);

			if (FAILED(hr)) {
				assert(srv);
				assert(resource);

				m_Textures[texture->mFilename.data] = nullptr;
				return false;
			}

			m_Textures[texture->mFilename.data] = srv;
		}
	}
	return true;
}

void ModelLoader::GetMaterials(const aiScene* scene)
{
	for (unsigned int mtrlidx = 0; mtrlidx < scene->mNumMaterials; mtrlidx++) {
		aiMaterial* material;

		material = scene->mMaterials[mtrlidx];

		GetMaterial(material);
	}
}

void ModelLoader::GetMaterial(const aiMaterial* material)
{
	auto device = Graphics::GetDevice();
	auto deviceContext = Graphics::GetDeviceContext();

	MATERIAL mtl;

	aiColor3D diffuse{ 0.0f, 0.0f, 0.0f };

	// Diffuse
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS) {
		mtl.Diffuse.x = diffuse.r;
		mtl.Diffuse.y = diffuse.g;
		mtl.Diffuse.z = diffuse.b;
		mtl.Diffuse.w = 1.0f;
	}
	else {
		mtl.Diffuse.x = 1.0f;
		mtl.Diffuse.y = 1.0f;
		mtl.Diffuse.z = 1.0f;
		mtl.Diffuse.w = 1.0f;
	}

	aiColor3D color{ 0.0f, 0.0f, 0.0f };

	// Specular
	if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
		mtl.Specular.x = color.r;
		mtl.Specular.y = color.g;
		mtl.Specular.z = color.b;
		mtl.Specular.w = 1.0f;
	}
	else {
		mtl.Specular.x = 0.0f;
		mtl.Specular.y = 0.0f;
		mtl.Specular.z = 0.0f;
		mtl.Specular.w = 0.0f;
	}

	// Shininess
	float shininess = 0.0f;
	if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
		mtl.Shininess = shininess;
	}
	else {
		mtl.Shininess = 0.0f;
	}

	aiColor3D ambient{ 0.0f, 0.0f, 0.0f };

	// Ambient
	if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS) {
		mtl.Ambient.x = ambient.r;
		mtl.Ambient.y = ambient.g;
		mtl.Ambient.z = ambient.b;
		mtl.Ambient.w = 1.0f;
	}
	else {
		mtl.Ambient.x = 0.0f;
		mtl.Ambient.y = 0.0f;
		mtl.Ambient.z = 0.0f;
		mtl.Ambient.w = 0.0f;
	}

	aiColor3D emissive{ 0.0f, 0.0f, 0.0f };

	// Emissive
	if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS) {
		mtl.Emission.x = emissive.r;
		mtl.Emission.y = emissive.g;
		mtl.Emission.z = emissive.b;
		mtl.Emission.w = 1.0f;
	}
	else {
		mtl.Emission.x = 0.0f;
		mtl.Emission.y = 0.0f;
		mtl.Emission.z = 0.0f;
		mtl.Emission.w = 0.0f;
	}

	aiTextureType type = aiTextureType_DIFFUSE;
	int texNum = material->GetTextureCount(type);

	assert(texNum <= 1);

	aiString str;

	material->GetTexture(type, 0, &str);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	if (m_Textures.find(str.data) == m_Textures.end()) {
		mtl.TextureEnable = FALSE;
		srv.Reset();

		material->GetTexture(type, 0, &str);

		std::string filename = utility::GetFileName(str.C_Str());

		srv = LoadDiffuseTexture(filename);

		if (!srv) {
			mtl.TextureEnable = FALSE;
		}
		else {
			mtl.TextureEnable = TRUE;
		}
	}
	else
	{
		mtl.TextureEnable = TRUE;
		srv = m_Textures[str.data];
	}

	m_AllTextures.emplace_back(srv);

	m_Materials.emplace_back(mtl);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ModelLoader::LoadDiffuseTexture(const std::string filename)
{
	std::string fileExtension = utility::GetFileExt(filename);

	std::wstring wideFileName = utility::GetWideFileName(filename);

	std::wstring parentDirectory = utility::GetParentDirectoryWide(m_FilePath);

	std::wstring filePath = parentDirectory + wideFileName;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	if (fileExtension == ".tga") {

		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filePath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image = std::make_unique<DirectX::ScratchImage>();

		HRESULT hr = LoadFromTGAFile(filePath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			std::cout << "LoadFromTGAFile Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		Microsoft::WRL::ComPtr<ID3D11Resource> tex;

		hr = CreateTexture(
			Graphics::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			tex.GetAddressOf());

		tex->Release();

		if (FAILED(hr)) {
			std::cout << "CreateTexture Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		hr = DirectX::CreateShaderResourceView(Graphics::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			srv.GetAddressOf());

		if (FAILED(hr)) {
			std::wcout << L"CreateShaderResourceView Error (" << filePath.c_str() << L")" << std::endl;
			return nullptr;
		}
	}
	else {
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			Graphics::GetDevice(),
			filePath.c_str(),
			nullptr,
			srv.GetAddressOf());
		if (FAILED(hr)) {
			std::wcout << L"CreateWICTextureFromFile Error (" << filePath.c_str() << " " << wideFileName.c_str() << L")" << std::endl;
			return nullptr;
		}
	}

	return srv;
}

