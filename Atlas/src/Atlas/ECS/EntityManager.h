#pragma once

#include "Atlas/Core/Core.h"
#include "ECSEntity.h"

#include <array>
#include <queue>

namespace ECS {

	class EntityManager {

	private:
		ECS::Entity m_EntityCount = 0;
		std::array<ECS::Signature, ECS::MAX_ENTITIES> m_Signatures;
		std::queue<ECS::Entity> m_RemovedEntities;

	public:

		EntityManager();

		ECS::Entity CreateEntity();
		void DestroyEntity(ECS::Entity entity);
		void SetSignature(ECS::Entity entity, ECS::Signature signature);
		ECS::Signature GetSignature(ECS::Entity entity);

	};

}
