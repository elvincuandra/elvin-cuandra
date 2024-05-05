#ifndef APP_H
#define APP_H

#include <Windows.h>
#include <stdint.h>

//! App class
/** This class is responsible for initializing and terminating the application.
 *  It also contains the main loop of the application.
 */	
class App
{
public:
	//! App constructor
	/** This constructor initializes the width and height of the application window.
	 *  @param width : width of the application window
	 *  @param height : height of the application window
	 */
	App(uint32_t width, uint32_t height);

	/** assignment operator<br>
	 *  This operator is deleted to prevent assignment of the App class.
	 */
	const App& operator=(const App&) = delete;

	/** copy constructor<br>
	 *  This constructor is deleted to prevent copy of the App class.
	 */
	App(const App&) = delete;

	//! App destructor
	~App();

public:
	/** Run function<br>
	 *  This function initializes the application window and the application itself, runs the main loop of the application, and terminates the application.
	 */
	void Run();

protected:
	//! InitWnd function<br>
	//! This function is called when the application window is initialized.
	//! @return true if the application window is initialized successfully, false otherwise
	bool InitWnd();
	//! TermWnd function<br>
	//! This function is called when the application window is terminated.
	void TermWnd();
	//! InitApp function<br>
	//! This function is called when the application is initialized.
	//! @return true if the application is initialized successfully, false otherwise
	bool InitApp();
	//! TermApp function<br>
	//! This function is called when the application is terminated.
	void TermApp();
	//! MainLoop function<br>
	//! This function is the main loop of the application.
	void MainLoop();
	//! WndProc function<br>
	//! This function is the callback function for the application window.
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	//! m_width<br>
	//! width of the application window
	uint32_t m_width;
	//! m_height<br>
	//! height of the application window
	uint32_t m_height;
	//! m_hWnd<br>
	//! handle to the application window
	HWND m_hWnd;
	//! m_hInst<br>
	//! handle to the application instance
	HINSTANCE m_hInst;
};

#endif // APP_H