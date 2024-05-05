#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include "Object.h"
#include <map>
#include <memory>
#include <string>

//! Scene class
/** This class is the base class for all scenes.
 * It is responsible for the initialization, update, rendering, and termination of objects contained within the container.
 */
class Scene
{
public:
	void InitBase() { 
		for (auto& e : m_objects) {
			e.second.get()->InitBase();
		}

		Init(); 
	};
	void UpdateBase(uint64_t deltaTime) {
		for (auto& e : m_objects) {
			e.second.get()->UpdateBase(deltaTime);
		}
		
		Update(deltaTime); 
	};
	void DrawBase(uint64_t deltaTime) { 
		for (auto& e : m_objects) {
			e.second.get()->DrawBase(deltaTime);
		}
		
		Draw(deltaTime); 
	};
	void UninitBase() { 
		for (auto& e : m_objects) {
			e.second.get()->UninitBase();
		}
		
		Uninit(); 
	};

protected:
	virtual void Init() = 0;
	virtual void Update(uint64_t deltaTime) = 0;
	virtual void Draw(uint64_t deltaTime) = 0;
	virtual void Uninit() = 0;

private:
	std::map<std::string, std::shared_ptr<Object>> m_objects; //!< This variable stores the objects in the scene.

protected:
	//! CreateObj function<br>
	/** This function creates an object of type T and adds it to the scene.
	 *	\tparam T refers to a child class of Object
	 *  @param objName : name of the object
	 */
	template <class T>
	T* CreateObj(const char* objName)
	{
		std::shared_ptr<T> object = std::make_shared<T>();
		m_objects.insert(std::pair<std::string, std::shared_ptr<Object>>(objName, object));
		object.get()->InitBase();
		return object.get();
	}

	//! GetObj function<br>
	/** This function returns an object of type T from the existing m_objects map container.
	 *	\tparam T refers to a child class of Object
	 *  @param objName : name of the object
	 */
	template <class T>
	T* GetObj(const char* objName) {
		auto it = m_objects.find(objName);
		if (it == m_objects.end()) return nullptr;

		std::shared_ptr<T> ptr = std::reinterpret_pointer_cast<T>(it->second);
		return ptr.get();
	}
};

#endif // SCENE_H