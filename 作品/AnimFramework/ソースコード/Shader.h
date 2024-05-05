#ifndef SHADER_H
#define SHADER_H

#include <wrl/client.h>
#include <string>
#include "dx11helper.h"
#include "Graphics.h"

//! Shader class
/** This class is responsible for managing the shader.
 *  It is responsible for setting the shader and the GPU.
 */
class Shader
{
public:
	//! SetShader function<br>
	/** This function sets the shader.
	 *  @param vs : vertex shader
	 *  @param ps : pixel shader
	 */
	void SetShader(std::string vs, std::string ps);
	//! SetGPU function<br>
	/** This function sets the shader to the GPU.
	*/
	void SetGPU();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader; //!< This variable stores the vertex shader.
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader; //!< This variable stores the pixel shader.
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout; //!< This variable stores the input layout.
public:
	//! GetVertexShader function<br>
	/** This function returns the pointer to the vertex shader.
	 *  @return pointer to the vertex shader
	 */
	ID3D11VertexShader* GetVertexShader() const { return m_pVertexShader.Get(); }

	//! GetPixelShader function<br>
	/** This function returns the pointer to the pixel shader.
	 *  @return pointer to the pixel shader
	 */
	ID3D11PixelShader* GetPixelShader() const { return m_pPixelShader.Get(); }

	//! GetInputLayout function<br>
	/** This function returns the pointer to the input layout.
	 *  @return pointer to the input layout
	 */
	ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout.Get(); }

};

#endif // SHADER_H