#include "PlayerModel.h"
#include "SkeletalMeshComponent.h"
#include "ModelLoader.h"
#include "ImGuiManager.h"

void PlayerModel::Init()
{
	auto skeletalMeshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent");
	skeletalMeshComponent->LoadModel("assets/model/kachujin.fbx");
	skeletalMeshComponent->LoadAnimation("assets/animations/Run.fbx", "Run");

	m_position = { 0.0f, 0.0f, 100.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };
	m_scale = { .5f, .5f, .5f };
}

void PlayerModel::Update(uint64_t deltaTime)
{
}

void PlayerModel::Draw(uint64_t deltaTime)
{
	Graphics::SetWorld(GetTransform());
}

void PlayerModel::Uninit()
{
}

void PlayerModel::ShowMeshStatistics()
{
	auto skeletalMeshComponent = GetComponent<SkeletalMeshComponent>("SkeletalMeshComponent");
	if(ImGui::Begin("Player Model Debug Window ")){

		if (ImGui::CollapsingHeader("Model Statistics")) {
			static float position[3] = { m_position.x, m_position.y, m_position.z };
			static float rotation[3] = { m_rotation.x, m_rotation.y, m_rotation.z };
			static float scale[3] = { m_scale.x, m_scale.y, m_scale.z };
			ImGui::DragFloat3("Position", position);
			ImGui::DragFloat3("Rotation", rotation);
			ImGui::DragFloat3("Scale", scale);
			this->SetTransform(position, rotation, scale);
		}
		
		if (ImGui::CollapsingHeader("Mesh Statistics")) {
			const auto& model = skeletalMeshComponent->GetModel();
			std::string mNumMesh = "Number of Meshes = " + std::to_string(model->GetSubsets().size());
			ImGui::SeparatorText(mNumMesh.c_str());
			for (auto& e : model->GetSubsets()) {
				ImGui::Text("Mesh Name : %s", e.Name.c_str());
				ImGui::Text("Number of Vertices : %d", e.VertexNum);
				ImGui::Text("Number of Indexes : %d", e.IndexNum);
				ImGui::Text("Base Vertex : %d", e.VertexBase);
				ImGui::Text("Base Index : %d", e.IndexBase);
				ImGui::Text("Material Index : %d", e.MaterialIdx);
				ImGui::Separator();
			}
		}

		// Todo : be able to manipulate each bone's translation data
		if (ImGui::CollapsingHeader("Bone Matrices")) {
			const auto& model = skeletalMeshComponent->GetModel();
			static float posArray[][3] = {0, 0, 0};
			static int e = 0;
			for (auto& bone : model->GetBones()) {
				std::string mNumBones = bone.first.c_str() + std::string("##2");
				std::string translationOffset = "Translation Offset " + std::to_string(e) + std::string("##1");
				if (ImGui::TreeNode(mNumBones.c_str())) {
					ImGui::DragFloat3(translationOffset.c_str(), posArray[e]);
					DirectX::XMFLOAT4X4 mtx;
					DirectX::XMFLOAT3 offset = DirectX::XMFLOAT3{posArray[e][0], posArray[e][1], posArray[e][2]};
					model->SetTranslation(bone.first.c_str(), offset); // Not Working
					DirectX::XMStoreFloat4x4(&mtx, model->GetBoneMatrix()[e]);
					PrintMatrices(mtx);
					ImGui::TreePop();
					ImGui::Separator();
				}
				e++;
			}
			e = 0;
		}

		if (ImGui::CollapsingHeader("Renderer Statistics")) {
			const auto &renderer = skeletalMeshComponent->GetRenderer();

			for (auto& e : renderer->GetSRV()) {
				ImGui::Text("Texture SRV Address : %p", (char*)e.Get());
				ImGui::Image(e.Get(), ImVec2(256, 256));
				ImGui::Separator();
			}
			for (auto& e : renderer->GetMaterials()) {
				ImGui::Text("Material CBuffer Address : %p", (char*)e->GetConstantBuffer().Get());
				ImGui::Separator();
			}
			auto e = renderer->GetVertexBuffer().Get();
			ImGui::Text("Vertex Buffer Address : %p", (char*)e);
			ImGui::Separator();

			e = renderer->GetIndexBuffer().Get();
			ImGui::Text("Index Buffer Address : %p", (char*)e);
			ImGui::Separator();

			e = renderer->GetBoneMtxBuffer();
			ImGui::Text("Bone Matrix Buffer Address : %p", (char*)e);
			ImGui::Separator();

		}

		if (ImGui::CollapsingHeader("Shader Statistics")) {
			const auto& shader = skeletalMeshComponent->GetShader();
			ImGui::Text("Vertex Shader Address : %p", (char*)shader->GetVertexShader());
			ImGui::Text("Pixel Shader Address : %p", (char*)shader->GetPixelShader());
			ImGui::Text("Input Layout Address : %p", (char*)shader->GetInputLayout());
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Animation Statistics")) {
			const auto& animation = skeletalMeshComponent->GetAnimation();
			for (auto& e : animation->GetAnimationContainer()) {
				ImGui::Text("Declared Animation Name = %s", e.first.c_str());
				for (unsigned int a = 0; a < e.second->mNumAnimations; a++) {
					const auto& anim = e.second->mAnimations[a];
					ImGui::Text("Imported Animation Name = %s", anim->mName.C_Str());
					ImGui::Text("Ticks per Second = %f", anim->mTicksPerSecond);
					ImGui::Text("Animation Duration = %f", anim->mDuration);
					ImGui::Separator();
				}
			}

			skeletalMeshComponent->PlayAnimationWindow();
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("World View Projection")) {
			
			ImGui::Text("World Matrix");
			DirectX::XMFLOAT4X4 world = Graphics::GetWorld();
			PrintMatrices(world);
			ImGui::Separator();

			ImGui::Text("View Matrix");
			DirectX::XMFLOAT4X4 view = Graphics::GetView();
			PrintMatrices(view);
			ImGui::Separator();

			ImGui::Text("Projection Matrix");
			DirectX::XMFLOAT4X4 proj = Graphics::GetProj();
			PrintMatrices(proj);
			ImGui::Separator();
		}

	}
	ImGui::End();
	
}

void PlayerModel::PrintMatrices(DirectX::XMFLOAT4X4& matrix)
{
	ImGui::Text("%f , %f, %f, %f", matrix._11, matrix._12, matrix._13, matrix._14);
	ImGui::Text("%f , %f, %f, %f", matrix._21, matrix._22, matrix._23, matrix._24);
	ImGui::Text("%f , %f, %f, %f", matrix._31, matrix._32, matrix._33, matrix._34);
	ImGui::Text("%f , %f, %f, %f", matrix._41, matrix._42, matrix._43, matrix._44);
}
