#include "atlpch.h"
#include "EntityManager.h"

namespace ECS {

	EntityManager::EntityManager()
	{
	}

	ECS::Entity EntityManager::CreateEntity()
		{
			ATL_CORE_ASSERT(m_EntityCount < ECS::MAX_ENTITIES, "Too many Entities");

			ECS::Entity e;
			if (m_RemovedEntities.empty())
			{
				e = m_EntityCount;
				m_EntityCount++;
			}
			else
			{
				e = m_RemovedEntities.front();
				m_RemovedEntities.pop();
			}

			return e;
		}

		void EntityManager::DestroyEntity(ECS::Entity entity)
		{
			ATL_CORE_ASSERT(entity < ECS::MAX_ENTITIES, "Entity out of range");

			m_Signatures[entity].reset();
			m_RemovedEntities.push(entity);
			m_EntityCount--;
		}

		void EntityManager::SetSignature(ECS::Entity entity, ECS::Signature signature)
		{
			ATL_CORE_ASSERT(entity < ECS::MAX_ENTITIES, "Entity out of range");

			m_Signatures[entity] = signature;
		}

		ECS::Signature EntityManager::GetSignature(ECS::Entity entity)
		{
			ATL_CORE_ASSERT(entity < ECS::MAX_ENTITIES, "Entity out of range");

			return m_Signatures[entity];
		}

}