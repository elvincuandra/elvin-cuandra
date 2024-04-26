#ifndef SKELETALMESHCOMPONENT_H
#define SKELETALMESHCOMPONENT_H

#include "Component.h"
#include <memory>
#include <string>

#include "ModelLoader.h"
#include "ModelRenderer.h"
#include "AnimationLoader.h"
#include "Shader.h"

#include <thread>

//! SkeletalMeshComponent class
/** This class is a subset of Component class responsible for loading the model, animation, and shader.
 *  It is also responsible for playing the animation.
 */
class SkeletalMeshComponent : public Component
{
protected:

	virtual void Init() override;
	virtual void Update(uint64_t deltaTime) override;
	virtual void Draw(uint64_t deltaTime) override;
	virtual void Uninit() override;

public:
	//! LoadModel function<br>
	/** This function loads the model.
	 *  @param fileName : name of the file
	 *  @return bool : returns true if the model is loaded successfully
	 */
	bool LoadModel(std::string fileName);

	//! LoadAnimation function<br>
	/** This function loads the animation.
	 *  @param fileName : name of the file
	 *  @param name : name of the animation (key for keeping the animation name)
	 */
	void LoadAnimation(std::string fileName, std::string name);
private:
	std::unique_ptr<ModelLoader> m_pModel; //!< pointer to ModelLoader class
	std::unique_ptr<ModelRenderer> m_pRenderer; //!< pointer to ModelRenderer class
	std::unique_ptr<AnimationLoader> m_pAnimation; //!< pointer to AnimationLoader class
	std::unique_ptr<Shader> m_pShader; //!< pointer to Shader class

	std::string m_AnimName; //!< name of the animation

	static int m_frame;
	static bool m_isPlaying;
	static float m_totalFrameTime;

	// ImGui Debug
public:
	//! PlayAnimationWindow function<br>
	/** This function opens an ImGui window for playing the animation.
	*/
	void PlayAnimationWindow();
private:
	//! FrameChecker function<br>
	/** This function adjusts the total frame time to the animation's length,
	 *  and keeps track of the current frame count.
	 *  @param frameNum : current frame count
	 */
	void FrameChecker(int& frameNum);

public:
// Getter
#pragma region Getter
	//! GetModel function<br>
	/** This function returns the model.
	 *  @return pointer to ModelLoader class
	 */
	ModelLoader* GetModel() const { return m_pModel.get(); }

	//! GetRenderer function<br>
	/**	This function returns the renderer.
	 *   @return pointer to ModelRenderer class
	 */
	ModelRenderer* GetRenderer() const { return m_pRenderer.get(); }

	//! GetAnimation function<br>
	/** This function returns the animation.
	 *  @return pointer to AnimationLoader class
	 */
	AnimationLoader* GetAnimation() const { return m_pAnimation.get(); }

	//! GetShader function<br>
	/** This function returns the shader.
	 *  @return pointer to Shader class
	 */
	Shader* GetShader() const { return m_pShader.get(); }
#pragma endregion
};

#endif // SKELETALMESHCOMPONENT_H