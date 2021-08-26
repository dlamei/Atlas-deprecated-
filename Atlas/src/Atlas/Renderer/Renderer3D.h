#pragma once

#include "PerspectiveCamera.h"
#include "Scene.h"

namespace Atlas {

	class Renderer3D
	{

	private:

	public:
		static void Init();
		static void Shutdown();

		static void DrawScene(const Ref<Scene> scene);
		static void EndScene();
		static void Flush(const Ref<Scene> scene);
	};

}
