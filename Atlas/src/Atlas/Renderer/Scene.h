#pragma once

#include "Atlas/Core/Core.h"

#include "PerspectiveCamera.h"
#include "PerspectiveCameraController.h"
#include "Mesh.h"

namespace Atlas {

	class Scene
	{
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		glm::vec3 m_Light = glm::vec3(0.0f);
		PerspectiveCameraController m_ActiveCamera;

	public:
		Scene(PerspectiveCameraController& camera);
		~Scene() {}
		
		static Ref<Scene> Create(PerspectiveCameraController& camera);

		inline void AddMesh(const Ref<Mesh> mesh) { m_Meshes.push_back(mesh); }
		inline void SetLight(const glm::vec3& light) { m_Light = light; }

		inline Ref<Mesh> GetMesh(uint32_t indx) { return m_Meshes.at(indx); }
		inline std::vector<Ref<Mesh>>& GetMeshVector() { return m_Meshes; }
		inline glm::vec3& GetLight() { return m_Light; }

		inline void SetActiveCamera(PerspectiveCameraController& camera) { m_ActiveCamera = camera; }
		inline PerspectiveCameraController& GetActiveCamera() { return m_ActiveCamera; }
		inline const PerspectiveCamera& getCamera() { return m_ActiveCamera.GetCamera(); }
	};

}

