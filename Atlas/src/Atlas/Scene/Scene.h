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
		PerspectiveCameraController m_ActiveCamera;

		ECS::Register m_Register;
		ECS::Entity m_SelectedEntity = ECS::null;
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
				CreateComponent<TagComponent>(entity, component.Mesh->GetName());
			}
		}

	public:
		Scene();
		Scene(PerspectiveCameraController& camera);
		~Scene() = default;

		std::set<ECS::Entity>& GetEntities() { return m_Entities; }
		MeshComponent& LoadMesh(const char* path);

		void SetActiveCamera(PerspectiveCameraController& camera) { m_ActiveCamera = camera; }
		PerspectiveCameraController& GetActiveCamera() { return m_ActiveCamera; }
		const PerspectiveCamera& getCamera() { return m_ActiveCamera.GetCamera(); }

		void SetSelectedEntity(ECS::Entity entity) { m_SelectedEntity = entity; }
		ECS::Entity GetSelectedEntity() { return m_SelectedEntity; }

		void OnUpdateEditor();

		const ECS::Entity CreateEntity(const char* tag = {})
		{

			ECS::Entity entity = m_Register.CreateEntity();
			m_Register.CreateComponent<TagComponent>(entity, tag ? tag : "Entity");
			m_Register.CreateComponent<IDComponent>(entity, entity);
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
		[[deprecated("use CreateComponent instead")]]
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

		template<typename T, typename... Args>
		inline T& CreateComponentEx(ECS::Entity entity, Args&&... args)
		{
			if (m_Register.IsComponentRegistered<T>())
			{
				if (m_Register.HasComponent<T>(entity))	return m_Register.GetComponent<T>(entity);
			}

			T& component = m_Register.CreateComponent<T>(entity, std::forward<Args>(args)...);
			OnComponentAdded<T>(entity);
			return component;
		}

		template<typename T>
		void RemoveComponent(ECS::Entity entity)
		{
			m_Register.RemoveComponent<T>(entity);
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

		template<typename T>
		bool IsComponentRegistered()
		{
			return m_Register.IsComponentRegistered<T>();
		}

	};


}

