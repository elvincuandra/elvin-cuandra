#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <vector>
#include "Graphics.h"
#include "dx11helper.h"
#include <assert.h>

//! IndexBuffer class
/** This class is responsible for creating and setting the index buffer.
 *  It uses the Graphics class to access the device and device context.
 */
class IndexBuffer {
public:
	//! Create function<br>
	/** This function creates the index buffer.
	 *  @param indices : vector of indices
	 */
	void Create(const std::vector<unsigned int>& indices) {
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = Graphics::GetDevice();
		
		HRESULT hr;
		hr = DirectXHelper::CreateIndexBuffer(
			pDevice.Get(),
			(unsigned int)indices.size(),
			(void*)indices.data(),
			m_pIndexBuffer.GetAddressOf()
		);

		assert(hr == S_OK);
	}

	//! SetGPU function<br>
	/** This function sets the index buffer to the GPU.
	*/
	void SetGPU(){
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = Graphics::GetDeviceContext();

		pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer; //!< This variable stores the index buffer.

public:
	//! Get function<br>
	/** This function returns the pointer to the index buffer.
	 *  @return pointer to the index buffer
	 */
	ID3D11Buffer* Get() const { return m_pIndexBuffer.Get(); }
};

#endif // INDEXBUFFER_H