#pragma once


namespace Atlas {

	class Renderer3D
	{

	private:

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
		static void Flush();

		//TEMP
		static void OnUpdate();

	};

}
