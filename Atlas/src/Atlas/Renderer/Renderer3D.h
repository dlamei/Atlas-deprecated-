#pragma once

//TEMP
#include "PerspectiveCamera.h"


namespace Atlas {

	class Renderer3D
	{

	private:

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();
		static void Flush();

		//TEMP
		static void OnUpdate();

	};

}
