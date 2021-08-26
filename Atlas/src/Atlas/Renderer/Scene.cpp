#include "atlpch.h"
#include "Scene.h"

namespace Atlas {

	Ref<Scene> Scene::Create(PerspectiveCameraController& camera)
	{
		return std::make_shared<Scene>(camera);
	}

	Scene::Scene(PerspectiveCameraController& camera)
		: m_ActiveCamera(camera)
	{

	}

}

