#pragma once

#include "PerspectiveCamera.h"
//TEMP
#include "Mesh.h"

namespace Atlas {

	class Renderer3D
	{

	private:

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const PerspectiveCamera& camera, bool shading);
		static void EndScene();
		static void Flush();
	};

}
