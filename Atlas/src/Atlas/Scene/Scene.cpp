#include "atlpch.h"
#include "Scene.h"

#include "Components.h"

namespace Atlas {

	Scene::Scene()
	{
		m_ShadowMap = FrameBuffer::Create({
				1024,
				1024,
				{
					FBTextureFormat::DEPTH24STENCIL8
				}
			});
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
		//for (auto& entity : GetComponentGroup<TransformComponent>())
		if (m_SelectedEntity != ECS::null && HasComponent<TransformComponent>(m_SelectedEntity))
		{
			TransformComponent& transform = GetComponent<TransformComponent>(m_SelectedEntity);

			if (HasComponent<MeshComponent>(m_SelectedEntity))
			{
				Mesh& mesh = GetComponent<MeshComponent>(m_SelectedEntity);
				mesh.SetTransfrom(transform.GetTransform());
			}

			if (HasComponent<PointLightComponent>(m_SelectedEntity))
			{
				PointLightComponent& light = GetComponent<PointLightComponent>(m_SelectedEntity);
				light.Position = transform.Translation;
			}

			if (HasComponent<DirLightComponent>(m_SelectedEntity))
			{
				DirLightComponent& light = GetComponent<DirLightComponent>(m_SelectedEntity);
				light.Direction = transform.Rotation;
				light.Position = transform.Translation;
			}
		}
	}


	/*
	* TODO Delete all entities when scene is destroyed
	*/


}

