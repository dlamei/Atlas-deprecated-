#pragma once

#include <unordered_map>
#include <memory>

#include "ComponentArray.h"


namespace ECS {

	class ComponentManager
	{
	private:
		std::unordered_map<const char*, ECS::ComponentType> m_ComponentTypes{};
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
		ECS::ComponentType m_ComponentTypeCount = 0;

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			//TODO: use hash code
			const char* typeName = typeid(T).name();

			ATL_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use");

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}

	public:


		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			ATL_CORE_ASSERT(m_ComponentTypes.find(typeName) == m_ComponentTypes.end(), "Component type is already registered");

			m_ComponentTypes.insert({ typeName, m_ComponentTypeCount });
			m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			m_ComponentTypeCount++;
		}

		template<typename T>
		ECS::ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			ATL_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use");

			return m_ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(ECS::Entity entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(ECS::Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		bool HasComponent(ECS::Entity entity, T component)
		{
			const char* typeName = typeid(T).name();
			return m_ComponentTypes.find(typeName) != m_ComponentTypes.end();
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
