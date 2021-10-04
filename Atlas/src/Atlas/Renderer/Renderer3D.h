#pragma once

#include "PerspectiveCamera.h"
#include "Atlas/Scene/Scene.h"
#include "Mesh.h"

namespace Atlas {

	class Renderer3D
	{
	private:
		static void DrawMesh(Mesh& mesh,  const uint32_t id, const uint32_t shadowMapID, const glm::mat4& lightSpace);

	public:
		static void Init();
		static void Shutdown();

		static void DrawScene(const Ref<Scene> scene, const glm::mat4& lightSpace);
		static void DrawLights(const Ref<Scene> scene);
		static void DrawLightDepthMap(Ref<Scene> scene, const glm::mat4& viewProjection);
		static void Flush(const Ref<VertexArray>& vertexArray, uint32_t triangleCount);
		static void DrawOutline(const Ref<Scene> scene, const glm::vec4& color, float thickness);

	};

}
