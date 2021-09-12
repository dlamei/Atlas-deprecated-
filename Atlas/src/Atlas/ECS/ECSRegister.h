#pragma once

#include <memory>
#include <utility>

#include "ComponentManager.h"
#include "EntityManager.h"

namespace ECS {

	class Register
	{
	private:
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager> m_EntityManager;

	public:

		Register()
			: m_ComponentManager(std::make_unique<ComponentManager>()), m_EntityManager(std::make_unique<EntityManager>())
		{
		}

		const Entity CreateEntity()
		{
			return m_EntityManager->CreateEntity();
		}

		void RemoveEntity(Entity entity)
		{
			m_EntityManager->DestroyEntity(entity);
			m_ComponentManager->EntityDestroyed(entity);
		}

		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			m_ComponentManager->AddComponent<T>(entity, component);

			Signature signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(entity, signature);
		}

		template<typename T, typename... Args>
		T& CreateComponent(Entity entity, Args&&... args)
		{
			T& component = m_ComponentManager->CreateComponent<T>(entity, std::forward<Args>(args)...);

			Signature signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			return component;
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			m_ComponentManager->RemoveComponent<T>(entity);

			Signature signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return m_ComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		std::shared_ptr<ComponentArray<T, ECS::MAX_ENTITIES>> GetComponentArray()
		{
			return m_ComponentManager->GetComponentArray<T>();
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			Signature signature = m_EntityManager->GetSignature(entity);
			return signature[m_ComponentManager->GetComponentType<T>()];
		}

	};

}
