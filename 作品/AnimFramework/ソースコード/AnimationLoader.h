#ifndef ANIMATIONLOADER_H
#define ANIMATIONLOADER_H

#include <string>
#include <unordered_map>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

//! AnimationLoader class
/** This class is responsible for loading animation data from file and storing it in a container.
 *  It uses Assimp library to load animation data from file.
 */
class AnimationLoader
{
public:
	/** LoadAnimation function
	 *  This function loads animation data from file and stores it in a container.
	 *  @param fileName : file name of the animation data
	 *  @param name : name of the animation data (user - declared)
	 */
	void LoadAnimation(const std::string fileName, const std::string name);

private:
	/** m_AnimationContainer<br>
	 * 	This container stores animation data.
	 * 	The key is the name of the animation data and the value is the pointer to the animation data
	 */
	std::unordered_map<std::string, const aiScene*> m_AnimationContainer;

public:
	/** GetAnimation function
	 *  This function returns a pointer to the animation data.
	 *  @param name : name of the animation data (key for m_AnimationContainer to use)
	 *  @param idx : index of the animation data
	 *  @return pointer to the animation data
	 */
	aiAnimation* GetAnimation(const char* name, int idx);

	/** GetAnimationContainer function
	 *  This function returns the animation data container.
	 *  @return m_AnimationContainer
	 */
	std::unordered_map<std::string, const aiScene*> GetAnimationContainer() const { return m_AnimationContainer; }

};

#endif // ANIMATIONLOADER_H
