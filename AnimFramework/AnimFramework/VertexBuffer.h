#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Graphics.h"
#include <vector>

//! VertexBuffer class
/** This class is responsible for creating and setting the vertex buffer.
 *  It uses the Graphics class to access the device and device context.
 *  \tparam T can accept any type of vertex data
 */
template <typename T>
class VertexBuffer
{
public:
	//! Create function<br>
	/** This function creates the vertex buffer.
	 * 	@param vertices : vector of vertices
	 */
	void Create(const std::vector<T>& vertices) {
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = Graphics::GetDevice();

		HRESULT hr = E_FAIL;

		hr = DirectXHelper::CreateVertexBufferWrite(
			pDevice.Get(),
			sizeof(T),
			(unsigned int)vertices.size(),
			(void*)vertices.data(),
			m_pVertexBuffer.GetAddressOf()
		);
		assert(hr == S_OK);
	
	}

	//! SetGPU function<br>
	/** This function sets the vertex buffer to the GPU.
	 * 	 
	 */
	void SetGPU() {
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();

		unsigned int stride = sizeof(T);
		unsigned int offset = 0;

		pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	}


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer; //!< This variable stores the vertex buffer.

public:
	/** returns a pointer to the vertex buffer*/
	ID3D11Buffer* Get() const { return m_pVertexBuffer.Get(); }
};

#endif // VERTEXBUFFER_H
