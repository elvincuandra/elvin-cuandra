#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "ImGuiManager.h"
#include "structs.h"

//! Camera class
/** This class is responsible for managing the camera.
 *  It inherits from Object class.
 */
class Camera : public Object
{
public:
	//! Camera constructor
	//! This function initializes the camera.
	virtual void Init() override;
	//! Update function<br>
	//! This function is called when the camera is updated.
	//! @param deltaTime : time passed since the last frame
	virtual void Update(uint64_t deltaTime) override;
	//! Draw function<br>
	//! This function is called when the camera is drawn.
	//! @param deltaTime : time passed since the last frame
	virtual void Draw(uint64_t deltaTime) override;
	//! Uninit function<br>
	//! This function is called when the camera is terminated.
	virtual void Uninit() override;
	//! ShowCameraStatistics function<br>
	//! This function shows the camera statistics in the ImGui window.
	void ShowCameraStatistics();

private:
	DirectX::XMMATRIX m_view; //!< This variable stores the view matrix.
	DirectX::XMMATRIX m_projection; //!< This variable stores the projection matrix.
	DirectX::XMVECTOR m_up = {0, 1, 0}; //!< This variable stores the up vector.

public:
	//! GetViewMatrix function<br>
	//! This function returns the view matrix.
	DirectX::XMMATRIX GetViewMatrix() const { return m_view; }
	//! GetProjectionMatrix function<br>
	//! This function returns the projection matrix.
	DirectX::XMMATRIX GetProjectionMatrix() const { return m_projection; }
};

#endif // CAMERA_H