#include "atlpch.h"
#include "Entity.h"

namespace Atlas {

	Entity::Entity(ECS::Entity handle, Scene* scene)
		: m_Handle(handle), m_Scene(scene)
	{
	}

	template<typename T>
	void Entity::AddComponent(T component)
	{
		m_Scene->m_Register.AddComponent<T>(m_Handle, component);
	}

	template<typename T>
	T& Entity::GetComonent()
	{
		return m_Scene->m_Register.GetComponent<T>(m_Handle);
	}

	template<typename T>
	bool Entity::HasComponent()
	{
		return m_Scene->m_Register.HasComponent<T>(m_Handle);
	}

	template<typename T>
	void Entity::RemoveComponent()
	{
		m_Scene->m_Register.RemoveComponent<T>(m_Handle);
	}


}
