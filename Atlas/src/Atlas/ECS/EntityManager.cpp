#include "atlpch.h"
#include "EntityManager.h"

namespace ECS {

	EntityManager::EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}

	ECS::Entity EntityManager::CreateEntity()
		{
			ATL_CORE_ASSERT(m_EntityCount < ECS::MAX_ENTITIES, "Too many Entities");

			Entity e = m_AvailableEntities.front();
			m_AvailableEntities.pop();
			m_EntityCount++;

			return e;
		}

		void EntityManager::DestroyEntity(ECS::Entity entity)
		{
			ATL_CORE_ASSERT(entity < ECS::MAX_ENTITIES, "Entity out of range");

			m_Signatures[entity].reset();
			m_AvailableEntities.push(entity);

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