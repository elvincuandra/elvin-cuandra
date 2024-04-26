#include "SkeletalMeshComponent.h"
#include "ImGuiManager.h"
#include "fpscontrol.h"

int SkeletalMeshComponent::m_frame = 0;
bool SkeletalMeshComponent::m_isPlaying = false;
float SkeletalMeshComponent::m_totalFrameTime = 0;

void SkeletalMeshComponent::Init()
{
	m_pShader = std::make_unique<Shader>();
	m_pShader->SetShader("shader/VertexShader.hlsl", "shader/PixelShader.hlsl");
}

void SkeletalMeshComponent::Update(uint64_t deltaTime)
{
	if (!m_pAnimation) return;

	static FPS fps;

	fps.SetFPS(m_pAnimation->GetAnimationContainer()[m_AnimName]->mAnimations[0]->mTicksPerSecond);

	FrameChecker(m_frame);

	if (m_pModel && m_pAnimation) {
		m_pModel->Update(m_pAnimation, m_AnimName.c_str(), m_frame);
	}

	m_pRenderer->Update(m_pModel);

	if (!m_isPlaying) return;

	m_frame++;

	fps.Wait();
}

void SkeletalMeshComponent::Draw(uint64_t deltaTime)
{
	if (!m_pAnimation) return;

	m_pShader->SetGPU();
	m_pRenderer->Draw();
}

void SkeletalMeshComponent::Uninit()
{
	m_pModel.reset();
	if (m_pRenderer->GetMaterials().size() != 0) {
		m_pRenderer->Uninit();
	}
	m_pRenderer.reset();
	m_pAnimation.reset();
	m_pShader.reset();
}

bool SkeletalMeshComponent::LoadModel(std::string fileName)
{
	if (m_pModel || m_pRenderer || m_pAnimation){
		m_pModel.reset();
		m_pRenderer.reset();
		m_pAnimation.reset();
	}

	m_pModel = std::make_unique<ModelLoader>();

	if (m_pModel->LoadModel(fileName)) {
		m_pRenderer = std::make_unique<ModelRenderer>();
		m_pRenderer->Init(m_pModel);
		return true;
	}

	return false;
}

void SkeletalMeshComponent::LoadAnimation(std::string fileName, std::string name)
{
	m_pAnimation = std::make_unique<AnimationLoader>();
	m_pAnimation->LoadAnimation(fileName, name);
	m_AnimName = name;
}

void SkeletalMeshComponent::PlayAnimationWindow()
{
	ImGui::SliderInt("Animation Window",&m_frame, 1, (int)m_totalFrameTime, "%d", ImGuiSliderFlags_None);
	ImGui::Separator();


	static std::string buttonText;
	if (m_isPlaying) {
		buttonText = "Pause Animation";
	}
	else {
		buttonText = "Play Animation";
	}

	if (ImGui::Button(buttonText.c_str(), ImVec2(125, 25))) {
		m_isPlaying = !m_isPlaying;
	}

	if (ImGui::Button("Previous Frame", ImVec2(125,25))) {
		FrameChecker(m_frame);
		m_frame--;
	}

	if (ImGui::Button("Next Frame", ImVec2(125, 25))) {
		FrameChecker(m_frame);
		m_frame++;
	}
	if (ImGui::Button("Reset Animation", ImVec2(125, 25))) {
		m_frame = 1;
	}
}

void SkeletalMeshComponent::FrameChecker(int& frameNum) {
	for (auto& e : m_pAnimation->GetAnimationContainer()) {
		m_totalFrameTime = e.second->mAnimations[0]->mDuration;
		if (frameNum >= m_totalFrameTime + 1 || frameNum <= 0) {
			frameNum = 1;
		}
	}
}