#pragma once

#include <unordered_map>
#include <memory>
#include <utility>

#include "ComponentArray.h"


namespace ECS {

	class ComponentManager
	{
	private:
		std::unordered_map<size_t, ECS::ComponentType> m_ComponentTypes{};
		std::unordered_map<size_t, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
		ECS::ComponentType m_ComponentTypeCount = 0;

	public:

		template<typename T>
		std::shared_ptr<ComponentArray<T, ECS::MAX_ENTITIES>> GetComponentArray()
		{
			size_t typeHash = typeid(T).hash_code();

			ATL_CORE_ASSERT(m_ComponentTypes.find(typeHash) != m_ComponentTypes.end(), "Component not registered before use");

			return std::static_pointer_cast<ComponentArray<T, ECS::MAX_ENTITIES>>(m_ComponentArrays[typeHash]);
		}


		template<typename T>
		void RegisterComponent()
		{
			size_t typeHash = typeid(T).hash_code();

			ATL_CORE_ASSERT(m_ComponentTypes.find(typeHash) == m_ComponentTypes.end(), "Component type is already registered");

			m_ComponentTypes.insert({ typeHash, m_ComponentTypeCount });
			m_ComponentArrays.insert({ typeHash, std::make_shared<ComponentArray<T, ECS::MAX_ENTITIES>>() });

			m_ComponentTypeCount++;
		}

		template<typename T>
		ECS::ComponentType GetComponentType()
		{
			size_t typeHash = typeid(T).hash_code();

			ATL_CORE_ASSERT(m_ComponentTypes.find(typeHash) != m_ComponentTypes.end(), "Component not registered before use");

			return m_ComponentTypes[typeHash];
		}

		template<typename T>
		void AddComponent(ECS::Entity entity, T component)
		{
			size_t typeHash = typeid(T).hash_code();
			if (m_ComponentTypes.find(typeHash) == m_ComponentTypes.end()) RegisterComponent<T>();

			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T, typename... Args>
		T& CreateComponent(Entity entity, Args&&... args)
		{
			size_t typeHash = typeid(T).hash_code();
			if (m_ComponentTypes.find(typeHash) == m_ComponentTypes.end()) RegisterComponent<T>();

			return GetComponentArray<T>()->CreateData(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent(ECS::Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		bool HasComponent(ECS::Entity entity, T component)
		{
			const char* typeHash = typeid(T).hash_code();
			return m_ComponentTypes.find(typeHash) != m_ComponentTypes.end();
		}

		template<typename T>
		T& GetComponent(ECS::Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(ECS::Entity entity)
		{
			for (auto const& c : m_ComponentArrays)
			{
				auto const& components = c.second;
				components->EntityDestroyed(entity);
			}
		}
	};

}
