#ifndef EDITOR_H
#define EDITOR_H

#include "Scene.h"

//! Editor class
//! This class is responsible for managing the editor.
//! It inherits from Scene class.
class Editor : public Scene
{
public:
	//! Editor constructor
	//! This function initializes the editor as well as the objects created within.
	virtual	void Init() override;
	//! Update function<br>
	//! This function is called when the editor is updated.
	//! @param deltaTime : time passed since the last frame
	virtual void Update(uint64_t deltaTime) override;
	//! Draw function<br>
	//! This function is called when the editor is drawn.
	//! @param deltaTime : time passed since the last frame
	virtual void Draw(uint64_t deltaTime) override;
	//! Uninit function<br>
	//! This function is called when the editor is terminated.
	virtual void Uninit() override;
};

#endif // EDITOR_H
