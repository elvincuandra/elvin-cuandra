#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d11.h>
#include <dxgi.h>
#include <stdint.h>
#include <wrl/client.h>
#include "structs.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

//! Graphics class (Renderer class)
/** This class is responsible for initializing, updating, drawing, and terminating the graphics.
 *  It initializes the graphics, sets the world, view, and projection matrices, and draws the graphics.
 *  It is essentially a wrapper for the Direct3D 11 API.
 */
class Graphics
{
public:
	Graphics() {};
	const Graphics& operator=(const Graphics&) = delete;
	Graphics(const Graphics&) = delete;
	~Graphics() {};
	
public:
	//! Init function<br>
	/** This function initializes the graphics (Direct3D11).
	 *  @param width : width of the window
	 *  @param height : height of the window
	 *  @param hWnd : handle to the window
	 *  @param vsync : whether to use vsync
	 *  @param fullscreen : whether to use fullscreen
	 *  @return S_OK if the graphics is initialized successfully, E_FAIL otherwise
	 */
	static HRESULT Init(uint32_t width, uint32_t height, HWND hWnd, bool vsync = false, bool fullscreen = false);
	//! Begin function<br>
	//! This function begins the rendering process.
	//! Clears the back buffer and depth buffer.
	static void Begin();
	//! Draw function<br>
	//! This function draws the graphics.
	static void End();
	static void Shutdown();

private:
	static ComPtr<ID3D11Device> m_pDevice; //!< This variable stores the device.
	static ComPtr<ID3D11DeviceContext> m_pDeviceContext; //!< This variable stores the device context.
	static ComPtr<IDXGISwapChain> m_pSwapChain; //!< This variable stores the swap chain.
	static ComPtr<ID3D11RenderTargetView> m_pRenderTargetView; //!< This variable stores the render target view.
	static ComPtr<ID3D11DepthStencilState> m_pDepthStencilState; //!< This variable stores the depth stencil state.
	static ComPtr<ID3D11DepthStencilView> m_pDepthStencilView; //!< This variable stores the depth stencil view.
	static ComPtr<ID3D11RasterizerState> m_pRasterizerState; //!< This variable stores the rasterizer state.
	static ComPtr<ID3D11SamplerState> m_pSamplerState; //!< This variable stores the sampler state.
	static ComPtr<ID3D11Buffer> m_pWorldBuffer; //!< This variable stores the world buffer.
	static ComPtr<ID3D11Buffer> m_pViewBuffer; //!< This variable stores the view buffer.
	static ComPtr<ID3D11Buffer> m_pProjBuffer; //!< This variable stores the projection buffer.
	static bool m_vsync; //!< This variable stores the vsync.
	static DirectX::XMFLOAT4X4 m_world;	//!< This variable stores the world matrix.
	static DirectX::XMFLOAT4X4 m_view; //!< This variable stores the view matrix.
	static DirectX::XMFLOAT4X4 m_proj; //!< This variable stores the projection matrix.

public:
#pragma region Getter
	//! GetDevice function<br>
	//! This function returns a pointer the device.
	static ID3D11Device* GetDevice() { return m_pDevice.Get(); }

	//! GetDeviceContext function<br>
	//! This function returns a pointer to the device context.
	static ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); }

	//! GetWorldMatrix function<br>
	//! This function returns the world matrix.
	static DirectX::XMFLOAT4X4 GetWorld() { return m_world; }

	//! GetViewMatrix function<br>
	//! This function returns the view matrix.
	static DirectX::XMFLOAT4X4 GetView() { return m_view; }

	//! GetProjMatrix function<br>
	//! This function returns the projection matrix.
	static DirectX::XMFLOAT4X4 GetProj() { return m_proj; }
#pragma endregion

#pragma region Setter

	//! SetWorld function<br>
	/** This function sets the world matrix.
	 *  @param world : world matrix
	 */ 
	static void SetWorld(DirectX::XMMATRIX world);
	//! SetView function<br>
	/** This function sets the view matrix.
	 * @param view : view matrix
	 */
	static void SetView(DirectX::XMMATRIX view);
	//! SetProj function<br>
	/** This function sets the projection matrix.
	 *  @param proj : projection matrix
	 */ 
	static void SetProj(DirectX::XMMATRIX proj);

#pragma endregion

};

#endif // GRAPHICS_H