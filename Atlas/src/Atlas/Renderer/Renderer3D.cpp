#include "atlpch.h"

#include "Renderer3D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Atlas {

	struct Renderer3DData
	{
		Ref<Shader> Shader;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		ATL_PROFILE_FUNCTION();

		s_Data.Shader = Shader::Create("assets/Shaders/Material.glsl");
		s_Data.Shader->Bind();
	}

	void Renderer3D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawScene(const Ref<Scene> scene)
	{
		ATL_PROFILE_FUNCTION();
		s_Data.Shader->Bind();

		s_Data.Shader->SetMat4("u_ViewProjection", scene->getCamera().GetViewProjectionMatrix());
		s_Data.Shader->SetFloat3("u_ViewPosition", scene->getCamera().GetPosition());

		s_Data.Shader->SetInt("material.DiffuseTexture", (int)Utils::TextureType::DIFFUSE);
		s_Data.Shader->SetInt("material.SpecularTexture", (int)Utils::TextureType::SPECULAR);

		
		int dirLightCount = 0;
		for (DirLightComponent& light : scene->GetComponentGroup<DirLightComponent>())
		{
			if (dirLightCount >= 4) break;
			light.SetUniform(s_Data.Shader, dirLightCount);
			dirLightCount++;
		}
		s_Data.Shader->SetInt("u_DirLightCount", dirLightCount);

		int pointLightCount = 0;
		for (PointLightComponent& light : scene->GetComponentGroup<PointLightComponent>())
		{
			if (pointLightCount >= 4) break;
			light.SetUniform(s_Data.Shader, pointLightCount);
			pointLightCount++;
		}
		s_Data.Shader->SetInt("u_PointLightCount", pointLightCount);

		for (MeshComponent& mesh : scene->GetComponentGroup<MeshComponent>())
		{
			if (!mesh.Hide)
			{
				s_Data.Shader->SetFloat("material.Shininess", mesh.Shininess);
				s_Data.Shader->SetMat4("u_TransformMatrix", mesh.Mesh->GetTransformMatrix());
				s_Data.Shader->SetInt("u_DisplayMode", (int) mesh.Mesh->GetDisplayMode());

				mesh.Mesh->BindTexture(Utils::TextureType::DIFFUSE);
				mesh.Mesh->BindTexture(Utils::TextureType::SPECULAR);

				Flush(mesh);
			}
		}


	}

	void Renderer3D::Flush(const Ref<Mesh>& mesh)
	{
		ATL_PROFILE_FUNCTION();
		mesh->GetVertexArray()->BindAll();

		RenderCommand::DrawIndexed(mesh->GetVertexArray(), mesh->GetTriangleCount() * 3);
	}
}
