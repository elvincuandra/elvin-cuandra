#pragma once

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <filesystem>
#include <DirectXMath.h>

//! Utility namespace
//! This namespace contains utility functions that are used in the project.
namespace utility {
	//! GetFileName function
	/** This function returns the file name from the file path.
	 *  @param filename : file path
	 *  @return file name
	 */
	std::string GetFileName(std::string filename);

	//! GetFileExt function
	/** This function returns the file extension from the file path.
	 *  @param filepath : file path
	 *  @return file extension
	 */
	std::string GetFileExt(std::string filepath);

	//! GetWideFileName function
	/** This function returns the wide file name from the file path.
	 *  @param filepath : file path
	 *  @return wide file name
	 */
	std::wstring GetWideFileName(std::string filepath);

	//! GetParentDirectoryWide function
	/** This function returns the wide parent directory from the file path.
	 *  @param filepath : file path
	 *  @return wide parent directory
	 */
	std::wstring GetParentDirectoryWide(std::string filepath);

	//! ConvertaiMtxTodxMtx function
	/** This function converts an aiMatrix4x4 to an XMMATRIX.
	 *  @param aiMtx : aiMatrix4x4
	 *  @return XMMATRIX
	 */
	DirectX::XMMATRIX ConvertaiMtxTodxMtx(const aiMatrix4x4& aiMtx);
}

