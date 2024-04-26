#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>

//! Input class
/** This class is responsible for handling input from the user.
 *  It is responsible for initializing, updating, and handling input from the user.
 */
class Input {
public:
	//! Init function<br>
	/** This function initializes the input. 	 
	*/
	static void Init();
	//! Update function<br>
	/** This function updates the input.
	*/ 
	static void Update();

public:
	//! IsKeyPress function<br>
	/** This function checks if a key is pressed.
 	*/
	static bool IsKeyPress(BYTE keyCode);
	//! IsKeyTrigger function<br>
	/** This function checks if a key is triggered.
	*/
	static bool IsKeyTrigger(BYTE keyCode);
	//! IsKeyRelease function<br>
	/** This function checks if a key is released.
	*/
	static bool IsKeyRelease(BYTE keyCode);

private:
	static BYTE m_keys[256]; //!< This variable stores the current state of the keys.
	static BYTE m_prevKeys[256]; //!< This variable stores the previous state of the keys.

};

#endif // INPUT_H