#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>

//! Component class
/** This class is the base class for all components.
 *  It is responsible for initializing, updating, drawing, and terminating the components.
 */
class Component
{
public:
	//! Component constructor
	//! This function is called when we want to initialize a child class.
	void InitBase() { Init(); };
	//! UpdateBase function<br>
	//! This function is called when we want to update a child class.
	//! @param deltaTime : time passed since the last frame
	void UpdateBase(uint64_t deltaTime) { Update(deltaTime); };
	//! DrawBase function<br>
	//! This function is called when we want to draw a child class.
	//! @param deltaTime : time passed since the last frame
	void DrawBase(uint64_t deltaTime) { Draw(deltaTime); };
	//! UninitBase function<br>
	//! This function is called when we want to terminate a child class.
	void UninitBase() { Uninit(); };

protected:
	//! Init function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	//! It is called when the component is initialized.
	virtual void Init() = 0;
	//! Update function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	//! It is called when the component is updated.
	virtual void Update(uint64_t deltaTime) = 0;
	//! Draw function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	//! It is called when the component is drawn.
	virtual void Draw(uint64_t deltaTime) = 0;
	//! Uninit function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	//! It is called when the component is terminated.
	virtual void Uninit() = 0;
};

#endif // COMPONENT_H
