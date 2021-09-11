#pragma once

#include "ECSEntity.h"
#include <array>
#include <unordered_map>

namespace ECS {

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(ECS::Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	private:
		std::array<T, ECS::MAX_ENTITIES> m_ComponentArray;
		std::unordered_map<ECS::Entity, size_t> m_EntityIndexMap;
		std::unordered_map<size_t, ECS::Entity> m_IndexEntityMap;

		size_t m_Size;

	public:

		void InsertData(ECS::Entity entity, T component)
		{
			ATL_CORE_ASSERT(m_EntityIndexMap.find(entity) == m_EntityIndexMap.end(), "Component can only be added once to an entity");

			m_EntityIndexMap[entity] = m_Size;
			m_IndexEntityMap[m_Size] = entity;
			m_ComponentArray[m_Size] = component;
			m_Size++;
		}

		void RemoveData(ECS::Entity entity)
		{
			ATL_CORE_ASSERT(m_EntityIndexMap.find(entity) != m_EntityIndexMap.end(), "Removing non-existant component");

			size_t removedEntityIndx = m_EntityIndexMap[entity];
			m_ComponentArray[removedEntityIndx] = m_ComponentArray[m_Size - 1];

			ECS::Entity lastEntity = m_IndexEntityMap[m_Size - 1];
			m_EntityIndexMap[static_cast<ECS::Entity>(m_Size - 1)] = removedEntityIndx;
			m_IndexEntityMap[removedEntityIndx] = lastEntity;

			m_EntityIndexMap.erase(entity);
			m_IndexEntityMap.erase(m_Size - 1);

			m_Size--;
		}

		T& GetData(ECS::Entity entity)
		{
			ATL_CORE_ASSERT(m_EntityIndexMap.find(entity) != m_EntityIndexMap.end(), "Retrieving non-existant component");
			return m_ComponentArray[m_EntityIndexMap[entity]];
		}

		void EntityDestroyed(ECS::Entity entity)
		{
			if (m_EntityIndexMap.find(entity) != m_EntityIndexMap.end())
			{
				RemoveData(entity);
			}
		}

	};

}
