#pragma once

#include "Atlas/Core/Core.h"

#include "Atlas/Renderer/Mesh.h"
#include "Atlas/Renderer/PerspectiveCameraController.h"

#include "Atlas/ECS/ECSRegister.h"
#include "Components.h"

#include <set>
#include <list>

namespace Atlas {

	class Scene
	{
	private:
		glm::vec3 m_Light = glm::vec3(0.0f);
		PerspectiveCameraController m_ActiveCamera;

		ECS::Register m_Register;
		std::set<ECS::Entity> m_Entities;
		std::set<ECS::Entity> m_ToRemove;

		template<typename T>
		void OnComponentAdded(ECS::Entity entity)
		{
		}

		template<>
		void OnComponentAdded<MeshComponent>(ECS::Entity entity)
		{
			MeshComponent& component = GetComponent<MeshComponent>(entity);
			if (HasComponent<TagComponent>(entity))
			{
				auto& tag = GetComponent<TagComponent>(entity);
				tag.Tag = component.Mesh->GetName();
			}
			else
			{
				AddComponent(entity, component.Mesh->GetName());
			}
		}

	public:
		Scene();
		Scene(PerspectiveCameraController& camera);
		~Scene() = default;

		inline std::set<ECS::Entity>& GetEntities() { return m_Entities; }

		void SetLight(const glm::vec3& light) { m_Light = light; }

		glm::vec3& GetLight() { return m_Light; }

		void SetActiveCamera(PerspectiveCameraController& camera) { m_ActiveCamera = camera; }
		PerspectiveCameraController& GetActiveCamera() { return m_ActiveCamera; }
		const PerspectiveCamera& getCamera() { return m_ActiveCamera.GetCamera(); }

		const ECS::Entity CreateEntity(const char* tag = {})
		{
			ECS::Entity entity = m_Register.CreateEntity();
			m_Register.CreateComponent<TagComponent>(entity, tag ? tag : "Entity");

			m_Entities.insert(entity);

			return entity;
		}

		void TagToRemove(ECS::Entity entity)
		{
			m_ToRemove.insert(entity);
		}

		void RemoveTaggedEntites()
		{
			for (ECS::Entity entity : m_ToRemove)
			{
				m_Entities.erase(entity);
				RemoveEntity(entity);
			}
			m_ToRemove.clear();
		}

		void RemoveEntity(ECS::Entity entity)
		{
			m_Register.RemoveEntity(entity);
			m_Entities.erase(entity);
		}

		template<typename T>
		inline void AddComponent(ECS::Entity entity, T& component)
		{
			m_Register.AddComponent<T>(entity, component);
			OnComponentAdded<T>(entity);
		}

		template<typename T, typename... Args>
		inline T& CreateComponent(ECS::Entity entity, Args&&... args)
		{
			T& component = m_Register.CreateComponent<T>(entity, std::forward<Args>(args)...);
			OnComponentAdded<T>(entity);
			return component;
		}

		template<typename T>
		void RemoveComponent(ECS::Entity entity, T comopnent)
		{
			m_Register.RemoveComponent<T>(entity, comopnent);
		}

		template<typename T>
		bool HasComponent(ECS::Entity entity)
		{
			return m_Register.HasComponent<T>(entity);
		}

		template<typename T>
		T& GetComponent(ECS::Entity entity)
		{
			return m_Register.GetComponent<T>(entity);
		}

		template<typename T>
		ECS::ComponentArray<T, ECS::MAX_ENTITIES>& GetComponentGroup()
		{
			return *m_Register.GetComponentArray<T>();
		}

	};


}

