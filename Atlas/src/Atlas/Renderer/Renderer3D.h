#pragma once

#include "PerspectiveCamera.h"
#include "Atlas/Scene/Scene.h"
#include "Mesh.h"

namespace Atlas {

	class Renderer3D
	{

	public:
		static void Init();
		static void Shutdown();

		static void DrawScene(const Ref<Scene> scene);
		static void Flush(const Ref<Mesh>& scene);

	};

}
