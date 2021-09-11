#pragma once

#include "Atlas/ECS/ECSRegister.h"
#include "Scene.h"

namespace Atlas {

	class Entity
	{
	private:
		ECS::Entity m_Handle = 0;
		Scene* m_Scene = nullptr;

	public:
		Entity() = default;
		Entity(ECS::Entity handle, Scene* scene);

		template<typename T>
		void AddComponent(T component);

		template<typename T>
		T& GetComonent();

		template<typename T>
		bool HasComponent();

		template<typename T>
		void RemoveComponent();

		operator bool() const { return m_Handle; }
		operator ECS::Entity() const { return m_Handle; }

		bool operator==(const Entity& other) const
		{
			return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	};

}
