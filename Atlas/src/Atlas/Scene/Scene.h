#pragma once

#include "Atlas/Core/Core.h"

#include "Atlas/Renderer/Mesh.h"
#include "Atlas/Renderer/PerspectiveCameraController.h"

#include "Atlas/ECS/ECSRegister.h"
#include "Components.h"

namespace Atlas {

	class Scene
	{
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		glm::vec3 m_Light = glm::vec3(0.0f);
		PerspectiveCameraController m_ActiveCamera;

		ECS::Register m_Register;
		std::vector<ECS::Entity> m_Entities;

	public:
		Scene();
		Scene(PerspectiveCameraController& camera);
		~Scene() = default;

		inline std::vector<ECS::Entity>& GetEntities() { return m_Entities; }

		void AddMesh(const Ref<Mesh> mesh) { m_Meshes.push_back(mesh); }
		void SetLight(const glm::vec3& light) { m_Light = light; }

		Ref<Mesh> GetMesh(uint32_t indx) { return m_Meshes.at(indx); }
		std::vector<Ref<Mesh>>& GetMeshVector() { return m_Meshes; }
		glm::vec3& GetLight() { return m_Light; }

		void SetActiveCamera(PerspectiveCameraController& camera) { m_ActiveCamera = camera; }
		PerspectiveCameraController& GetActiveCamera() { return m_ActiveCamera; }
		const PerspectiveCamera& getCamera() { return m_ActiveCamera.GetCamera(); }

		const ECS::Entity CreateEntity(const char* tag)
		{
			//Entity entity = { m_Register.CreateEntity(), this };
			//TagComponent tagComponent = { tag ? tag : "Entity" };
			//entity.AddComponent<TagComponent>(tagComponent);
			//return entity;
			ECS::Entity entity = m_Register.CreateEntity();

			TagComponent tagComponent = { tag ? tag : "Entity" };
			m_Register.AddComponent<TagComponent>(entity, tagComponent);
			m_Entities.push_back(entity);

			return entity;
		}

		void RemoveEntity(ECS::Entity entity)
		{
			m_Register.RemoveEntity(entity);
		}

		template<typename T>
		inline void AddComponent(ECS::Entity entity, T component)
		{
			m_Register.AddComponent<T>(entity, component);
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

	};


}

