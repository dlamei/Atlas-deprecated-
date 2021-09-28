#pragma once

#include "PerspectiveCamera.h"
#include "Atlas/Scene/Scene.h"
#include "Mesh.h"

namespace Atlas {

	class Renderer3D
	{
	private:
		static void DrawMesh(Mesh& mesh,  const uint32_t id);

	public:
		static void Init();
		static void Shutdown();

		static void DrawScene(const Ref<Scene> scene);
		static void Flush(const Mesh& mesh);

		static void DrawOutline(const Ref<Mesh>& mesh, const glm::mat4& viewProjMat, const glm::vec4& color, float thickness);

	};

}
