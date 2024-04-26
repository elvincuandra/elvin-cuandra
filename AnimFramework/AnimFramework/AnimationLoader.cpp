#include "AnimationLoader.h"
#include <assert.h>
#include <iostream>

void AnimationLoader::LoadAnimation(const std::string fileName, const std::string name)
{
	static Assimp::Importer importer;

	m_AnimationContainer[name] = importer.ReadFile(
		fileName.c_str(),
		aiProcess_ConvertToLeftHanded
	);

	assert(m_AnimationContainer[name]);

	if (m_AnimationContainer[name] == nullptr) {
		std::cout<< "load error " << fileName.c_str() << " ( " << importer.GetErrorString() << " )" << std::endl;
	}

}

aiAnimation* AnimationLoader::GetAnimation(const char* name, int idx)
{
	aiAnimation* animation = m_AnimationContainer[name]->mAnimations[idx];

	return animation;
}
