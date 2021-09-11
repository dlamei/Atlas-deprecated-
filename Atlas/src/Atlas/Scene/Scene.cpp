#include "atlpch.h"
#include "Scene.h"

#include "Components.h"

namespace Atlas {

	Scene::Scene()
	{
		m_Register.RegisterComponent<TagComponent>();
	}


	Scene::Scene(PerspectiveCameraController& camera)
		: m_ActiveCamera(camera)
	{
		m_Register.RegisterComponent<TagComponent>();
	}

	/*
	* TODO Delete all entities when scene is destroyed
	*/


}

