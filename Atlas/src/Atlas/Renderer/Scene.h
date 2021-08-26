#pragma once

#include "Atlas/Core/Core.h"

#include "PerspectiveCamera.h"
#include "PerspectiveCameraController.h"
#include "Mesh.h"

namespace Atlas {

	class Scene
	{
	private:
		Ref<Mesh> m_Mesh;
		glm::vec3 m_Light = glm::vec3(0.0f);
		PerspectiveCameraController m_ActiveCamera;

	public:
		Scene(PerspectiveCameraController& camera);
		~Scene() {}
		
		static Ref<Scene> Create(PerspectiveCameraController& camera);

		inline void SetMesh(const Ref<Mesh> mesh) { m_Mesh = mesh; }
		inline void SetLight(const glm::vec3& light) { m_Light = light; }

		inline Ref<Mesh> GetMesh() { return m_Mesh; }
		inline glm::vec3& GetLight() { return m_Light; }

		inline void SetActiveCamera(PerspectiveCameraController& camera) { m_ActiveCamera = camera; }
		inline PerspectiveCameraController& GetActiveCamera() { return m_ActiveCamera; }
		inline const PerspectiveCamera& getCamera() { return m_ActiveCamera.GetCamera(); }
	};

}

