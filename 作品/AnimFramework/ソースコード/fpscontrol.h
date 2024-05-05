#ifndef _FPS_CONTROL_H_
#define _FPS_CONTROL_H_

#include <windows.h>
#include <iostream>
#include <thread>

#pragma comment (lib, "winmm.lib")

//! FPS class
/** This class is responsible for controlling the frame rate of the application.
 *  It calculates the time passed since the last frame and waits for the remaining time to meet the desired frame rate.
 *  It has two options : VFR and CFR.
 *  VFR (Variable Frame Rate) : The frame rate is not fixed. It is calculated dynamically.
 *  CFR (Constant Frame Rate) : The frame rate is fixed. It is set by the user.
 *  CFR uses thread to wait for the remaining time to meet the desired frame rate.
 */
class FPS {
public:
	//! FPS constructor
	/* This constructor initializes with VFR by default. */ 
	FPS() {};

	//! FPS constructor
	/** This constructor initializes with CFR
	 *  @param f refers to the user's desired frame rate.
	 */
	FPS(uint64_t f) {
		m_MicrosecondsPerFrame = static_cast<uint64_t>((1000.f * 1000.f) / f);
	}

	//! Calculates the frame time between previous frame and current frame.
	//! @return the time passed since the last frame (deltaTime).
	uint64_t CalcDelta() {
		m_currentTime = timeGetTime();
		m_deltaTime = m_currentTime - m_lastTime;
		m_lastTime = m_currentTime;
		return m_deltaTime;
	};

	//! Sets the frame rate.
	//! @param f refers to the user's desired frame rate.
	void SetFPS(uint64_t f) {
		m_MicrosecondsPerFrame = static_cast<uint64_t>((1000.f * 1000.f) / f);
	};

	/** Waits for the remaining time to meet the desired frame rate.
	 *  This function uses thread to wait for the remaining time to meet the desired frame rate.
	 *  Sleeps the thread for the difference between the desired time passed for every frame and the deltaTime.
	 */
	void Wait() {
		int64_t sleepTime = m_MicrosecondsPerFrame - m_deltaTime;
		if (sleepTime > 0) {
			float tt = sleepTime / 1000.f;

#if defined(DEBUG) | defined(_DEBUG)
			// std::cout << "sleep time(ms): " << tt << std::endl;
#endif

			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
		}
	};

private:
	uint64_t m_MicrosecondsPerFrame = 0; //!< microseconds per frame. (only used for CFR)
	uint64_t m_currentTime = 0; //!< current time
	uint64_t m_lastTime = 0; //!< last time
	uint64_t m_deltaTime = 0; //!< time passed since the last frame
};

#endif // _FPS_CONTROL_H_