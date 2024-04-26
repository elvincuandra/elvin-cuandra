#ifndef OBJECT_H
#define OBJECT_H

#include "Component.h"
#include <map>
#include <string>
#include <memory>
#include <DirectXMath.h>

//! Object class
/** This class is the base class for all objects.
 *  It is responsible for the initialization, update, rendering, and termination of components contained within the container, as well as Object's child classes'.
 */
class Object
{
public:
	virtual ~Object() {};

public:
	//! InitBase function<br>
	/** This function is called when we want to initialize a child class.
	 *  As well as the components held within each child's class.
	 */
	void InitBase() { 

		for (auto& component : m_components)
		{
			component.second->InitBase();
		}

		Init(); 
	};

	//! UpdateBase function<br>
	/** This function is called when we want to update a child class.
	 *  As well as the components held within each child's class.
	 *  @param deltaTime : time passed since the last frame
	 */
	void UpdateBase(uint64_t deltaTime) {

		for (auto& component : m_components)
		{
			component.second->UpdateBase(deltaTime);
		}

		Update(deltaTime); 

	};

	//! DrawBase function<br>
	/** This function is called when we want to draw a child class.
	 *  As well as the components held within each child's class.
	 *  @param deltaTime : time passed since the last frame
	 */
	void DrawBase(uint64_t deltaTime) {

		for (auto& component : m_components)
		{
			component.second->DrawBase(deltaTime);
		}

		Draw(deltaTime); 

	};

	//! UninitBase function<br>
	/** This function is called when we want to terminate a child class.
	 *  As well as the components held within each child's class.
	 */
	void UninitBase() {

		for (auto& component : m_components)
		{
			component.second->UninitBase();
		}

		Uninit(); 

	};

protected:
	//! Init function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	virtual void Init() = 0;
	//! Update function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	virtual void Update(uint64_t deltaTime) = 0;
	//! Draw function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	virtual void Draw(uint64_t deltaTime) = 0;
	//! Uninit function<br>
	//! This function is a pure virtual function that calls its child classes when called.
	virtual void Uninit() = 0;

private:
	std::map<std::string, std::shared_ptr<Component>> m_components; //!< This variable stores the components in the object.

protected:
	//! CreateComponent function<br>
	/** This function creates a component of type T and adds it to the object.
	 *  \tparam T refers to a child class of Component
	 *  @param componentName : name of the component
	 * 
	 */
	template <class T>
	T* CreateComponent(const char* componentName)
	{
		std::shared_ptr<T> component = std::make_shared<T>();
		m_components.insert(std::pair < std::string, std::shared_ptr<Component> >(componentName, component));
		component.get()->InitBase();
		return component.get();
	}

	//! GetComponent function<br>
	/** This function returns a component of type T from the existing m_components map container.
	 *  \tparam T refers to a child class of Component
	 *  @param componentName : name of the component
	 */
	template <class T>
	T* GetComponent(const char* componentName) {
		auto it = m_components.find(componentName);
		if (it != m_components.end())
		{
			return dynamic_cast<T*>(it->second.get());
		}
		return nullptr;
	}

protected:
	DirectX::XMFLOAT3 m_position; //!< This variable stores the position of the object.
	DirectX::XMFLOAT3 m_rotation; //!< This variable stores the rotation of the object.
	DirectX::XMFLOAT3 m_scale; //!< This variable stores the scale of the object.

public:
#pragma region Getter
	//! GetPosition function<br>
	/** This function returns the position of the object.
	 *  @return position of the object
	 */
	DirectX::XMFLOAT3 GetPosition() const { return m_position; }

	//! GetRotation function<br>
	/** This function returns the rotation of the object.
	 *  @return rotation of the object
	 */
	DirectX::XMFLOAT3 GetRotation() const { return m_rotation; }

	//! GetScale function<br>
	/** This function returns the scale of the object.
	 *   @return scale of the object
	 */
	DirectX::XMFLOAT3 GetScale() const { return m_scale; }

	//! GetPositionXM function<br>
	/** This function returns the position of the object in XMVECTOR format.
	 *  @return position of the object in XMVECTOR format
	 */
	DirectX::XMVECTOR GetPositionXM() const { return DirectX::XMLoadFloat3(&m_position); }

	//! GetRotationXM function<br>
	/** This function returns the rotation of the object in XMVECTOR format.
	 *  @return rotation of the object in XMVECTOR format
	 */
	DirectX::XMVECTOR GetRotationXM() const { return DirectX::XMLoadFloat3(&m_rotation); }

	//! GetScaleXM function<br>
	/** This function returns the scale of the object in XMVECTOR format.
	 *   @return scale of the object in XMVECTOR format
	 */
	DirectX::XMVECTOR GetScaleXM() const { return DirectX::XMLoadFloat3(&m_scale); }

	//! GetTransform function<br>
	/** This function returns the transformation matrix of the object.
	 * 	@return transformation matrix of the object
	 */
	DirectX::XMMATRIX GetTransform() const
	{
		return DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rotation.y, m_rotation.x, m_rotation.z) *
			DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	}
#pragma endregion

	
#pragma region Setter
	//! SetPosition function<br>
	/** This function sets the position of the object.
	 *  @param position : position of the object
	 */
	void SetPosition(const DirectX::XMFLOAT3 position) { m_position = position; }

	//! SetRotation function<br>
	/** This function sets the rotation of the object.
	 *  @param rotation : rotation of the object
	 */
	void SetRotation(const DirectX::XMFLOAT3 rotation) { m_rotation = rotation; }

	//! SetScale function<br>
	/** This function sets the scale of the object.
	 *   @param scale : scale of the object
	 */
	void SetScale(const DirectX::XMFLOAT3 scale) { m_scale = scale; }

	//! SetTransform function<br>
	/** This function sets the transform of the object.
	 *  @param position : position of the object
	 *  @param rotation : rotation of the object
	 *  @param scale : scale of the object
	 */
	void SetTransform(const float* position, const float* rotation, const float* scale)
	{
		m_position = DirectX::XMFLOAT3(position[0], position[1], position[2]);
		m_rotation = DirectX::XMFLOAT3(rotation[0], rotation[1], rotation[2]);
		m_scale = DirectX::XMFLOAT3(scale[0], scale[1], scale[2]);
	}
#pragma endregion
};

#endif // OBJECT_H
