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

	MeshComponent& Scene::LoadMesh(const char* path)
	{
		ECS::Entity entity = CreateEntity();
		auto& component = CreateComponent<MeshComponent>(entity, path);
		CreateComponent<TransformComponent>(entity);
		return component;
	}

	void Scene::OnUpdateEditor()
	{
		for (auto& entity : GetComponentGroup<TransformComponent>())
		{
			if (HasComponent<MeshComponent>(entity))
			{
				Mesh& mesh = GetComponent<MeshComponent>(entity);
				mesh.SetTransfrom(entity.Component->GetTransform());
			}
		}
	}


	/*
	* TODO Delete all entities when scene is destroyed
	*/


}

