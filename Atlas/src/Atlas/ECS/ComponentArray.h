#pragma once

#include "ECSEntity.h"
#include <array>
#include <unordered_map>
#include <utility>

namespace ECS {

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(ECS::Entity entity) = 0;
	};

	template<typename T, size_t Size>
	class ComponentArray : public IComponentArray
	{
	public:
		
		struct EntityObj
		{
			T* Component;
			ECS::Entity EntityHandle;

			EntityObj() = default;
			EntityObj(T* component, ECS::Entity entity)
				: Component(component), EntityHandle(entity) {}

			operator T&() { return *Component; }
			operator ECS::Entity() { return EntityHandle; }
		};

		struct Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = EntityObj;
			using Pointer = value_type*;
			using Reference = value_type&;

		private:
			value_type m_Pair;
			T* m_Ptr;
			int m_Index = 0;
			std::unordered_map<size_t, ECS::Entity> m_IndexEntityMap;
		public:

			Iterator(T* ptr, std::unordered_map<size_t, ECS::Entity> indexEntityMap)
				: m_Ptr(ptr), m_IndexEntityMap(indexEntityMap), m_Pair(value_type(ptr, indexEntityMap[m_Index]))
			{
			}

			Reference operator*() { return m_Pair; }
			Pointer operator->() { return &m_Ptr; }

			Iterator& operator++()
			{
				m_Ptr++;
				m_Index++;
				m_Pair.Component = m_Ptr;
				m_Pair.EntityHandle = m_IndexEntityMap[m_Index];
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator== (const Iterator& a, const Iterator& b)
			{
				return a.m_Ptr == b.m_Ptr;
			}

			friend bool operator!= (const Iterator& a, const Iterator& b)
			{
				return a.m_Ptr != b.m_Ptr;
			}
		};


	public:
		//TODO: maybe add std::vector
		T m_ComponentArray[Size];
		std::unordered_map<ECS::Entity, size_t> m_EntityIndexMap;
		std::unordered_map<size_t, ECS::Entity> m_IndexEntityMap;

		size_t m_Size = 0;

	public:

		Iterator begin() { return Iterator(&m_ComponentArray[0], m_IndexEntityMap); }
		Iterator end() { return Iterator(&m_ComponentArray[m_Size], m_IndexEntityMap); }

		void InsertData(ECS::Entity entity, T component)
		{
			ATL_CORE_ASSERT(m_EntityIndexMap.find(entity) == m_EntityIndexMap.end(), "Component can only be added once to an entity");

			m_EntityIndexMap[entity] = m_Size;
			m_IndexEntityMap[m_Size] = entity;
			m_ComponentArray[m_Size] = component;
			m_Size++;
		}

		template<typename... Args>
		T& CreateData(ECS::Entity entity, Args&&... args)
		{
			ATL_CORE_ASSERT(m_EntityIndexMap.find(entity) == m_EntityIndexMap.end(), "Component can only be added once to an entity");

			m_EntityIndexMap[entity] = m_Size;
			m_IndexEntityMap[m_Size] = entity;
			m_ComponentArray[m_Size] = T(args...);
			m_Size++;

			return m_ComponentArray[m_Size - 1];
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
