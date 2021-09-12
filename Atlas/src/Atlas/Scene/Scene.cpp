#include "atlpch.h"
#include "Scene.h"

#include "Components.h"

namespace Atlas {

	Scene::Scene()
	{
	}


	Scene::Scene(PerspectiveCameraController& camera)
		: m_ActiveCamera(camera)
	{
	}


	/*
	* TODO Delete all entities when scene is destroyed
	*/


}

