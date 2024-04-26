#pragma once

#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

//! DirectXHelper namespace<br>
//! This namespace contains helper functions for DirectX.
//! It contains functions for creating constant buffer, vertex buffer, index buffer, vertex shader, and pixel shader.
//! It also contains functions for compiling shader and reading shader.
namespace DirectXHelper {
	//! CreateConstantBuffer function<br>
	/** This function creates a constant buffer.
	 *  @param device : pointer to the device
	 *  @param byteSize : size of the buffer in bytes
	 *  @param constantBuffer : pointer to the buffer
	 *  @return S_OK if the buffer is created successfully, E_FAIL otherwise
	 */
	HRESULT CreateConstantBuffer(ID3D11Device* device, unsigned int byteSize, ID3D11Buffer** constantBuffer);

	//! CreateConstantBufferWrite function<br>
	/** This function creates a constant buffer and writes data to it.
	 *  @param device : pointer to the device
	 *  @param byteSize : size of the buffer in bytes
	 *  @param constantBuffer : pointer to the buffer
	 *  @return S_OK if the buffer is created successfully, E_FAIL otherwise
	 */
	HRESULT CreateConstantBufferWrite(ID3D11Device* device, unsigned int byteSize, ID3D11Buffer** constantBuffer);

	//! CreateVertexBuffer function<br>
	/** This function creates a vertex buffer.
	 *  @param device : pointer to the device
	 *  @param stride : stride of the buffer
	 *  @param vertexNum : number of vertices
	 *  @param vertexData : pointer to the vertex data
	 *  @param vertexBuffer : pointer to the buffer
	 *  @return S_OK if the buffer is created successfully, E_FAIL otherwise
	 */
	HRESULT CreateVertexBufferWrite(ID3D11Device* device, unsigned int stride, unsigned int vertexNum, void* vertexData, ID3D11Buffer** vertexBuffer);

	//! CreateIndexBuffer function<br>
	/** This function creates an index buffer.
	  * @param device : pointer to the device
	  * @param indexNum : number of indices
	  * @param indexData : pointer to the index data
	  * @param indexBuffer : pointer to the buffer
	  * @return S_OK if the buffer is created successfully, E_FAIL otherwise
	  */
	HRESULT CreateIndexBuffer(ID3D11Device* device, unsigned int indexNum, void* indexData, ID3D11Buffer** indexBuffer);

	//! CreateVertexShader function<br>
	/** This function creates a vertex shader.
	 *  @param device : pointer to the device
	 *  @param fileName : file name of the shader
	 *  @param entryPoint : entry point of the shader
	 *  @param shaderModel : shader model of the shader
	 *  @param layout : pointer to the input layout
	 *  @param numElements : number of elements
	 *  @param ppVertexShader : pointer to the vertex shader
	 *  @param ppVertexLayout : pointer to the input layout
	 *  @return S_OK if the shader is created successfully, E_FAIL otherwise
	 */
	HRESULT CreateVertexShader(ID3D11Device* device, const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout);

	//! CreatePixelShader function<br>
	/** This function creates a pixel shader.
	 *  @param device : pointer to the device
	 *  @param fileName : file name of the shader
	 *  @param entryPoint : entry point of the shader
	 *  @param shaderModel : shader model of the shader
	 *  @param pixelShader : pointer to the pixel shader
	 *  @return S_OK if the shader is created successfully, E_FAIL otherwise
	 */
	HRESULT CreatePixelShader(ID3D11Device* device, const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3D11PixelShader** pixelShader);

	//! CompileShader function<br>
	/** This function compiles a shader.
	 *  @param fileName : file name of the shader
	 *  @param entryPoint : entry point of the shader
	 *  @param shaderModel : shader model of the shader
	 *  @param shaderObj : pointer to the shader object
	 *  @param ShaderObjSize : size of the shader object
	 *  @param ppBlob : pointer to the blob
	 *  @return S_OK if the shader is compiled successfully, E_FAIL otherwise
	 */
	HRESULT CompileShader(const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, void** shaderObj, size_t& ShaderObjSize, ID3DBlob** ppBlob);

	//! CompileShaderFromFile function<br>
	/** This function compiles a shader from file.
	 *  @param fileName : file name of the shader
	 *  @param entryPoint : entry point of the shader
	 *  @param shaderModel : shader model of the shader
	 *  @param ppBlob : pointer to the blob
	 *  @return S_OK if the shader is compiled successfully, E_FAIL otherwise
	 */
	HRESULT CompileShaderFromFile(const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlob);

	//! ReadShader function<br>
	/** This function reads a shader.
	 *  @param csoName : file name of the shader
	 *  @param byteArray : vector of unsigned char
	 *  @return S_OK if the shader is read successfully, E_FAIL otherwise
	 */
	HRESULT ReadShader(const char* csoName, std::vector<unsigned char>& byteArray);

	//! GetFileExtName function<br>
	/** This function returns the file extension name.
	 *  @param filename : file name
	 *  @return file extension name
	 */
	std::string GetFileExtName(const char* filename);
}