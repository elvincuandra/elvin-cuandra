#ifndef MANAGER_H
#define MANAGER_H

#include <stdint.h>
#include <Windows.h>
#include <memory>
#include "Scene.h"

//! Manager class
/** This class is responsible for managing the application.
 *  It controls the flow of the application
 */ 
class Manager
{
public:
	//! Init function<br>
	/** This function initializes the application.
	 *  @param width : width of the application window
	 *  @param height : height of the application window
	 *  @param hWnd : handle to the application window
	 */
	static void Init(uint32_t width, uint32_t height, HWND hWnd);
	//! Update function<br>
	/** This function updates the application.
	 *  @param deltaTime : time passed since the last frame
	 */
	static void Update(uint64_t deltaTime);
	//! Draw function<br>
	/** This function draws the application.
	 *  @param deltaTime : time passed since the last frame
	 */
	static void Draw(uint64_t deltaTime);
	//! Uninit function<br>
	/** This functions terminates the application.
	*/
	static void Uninit();

private:
	static std::unique_ptr<Scene> m_pScene; //!< pointer to scene class

protected:
	/** 
     * \brief Initialize Scene
     * \tparam T refers to a child class of Scene
     */
	template <typename T>
	static void SetScene()
	{
		if (m_pScene) {
			m_pScene->UninitBase();
			m_pScene.reset();
		}
		m_pScene = std::make_unique<T>();
		m_pScene->InitBase();
	}
};

#endif // MANAGER_H