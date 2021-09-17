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

		s_Data.Shader->SetFloat("material.Shininess", 8.0f);

		s_Data.Shader->SetFloat3("dirLight.Direction", {-0.2f, -1.0f, -0.3f});
		s_Data.Shader->SetFloat3("dirLight.Ambient",   { 0.2f,  0.2f,  0.2f });
		s_Data.Shader->SetFloat3("dirLight.Diffuse",   { 1.0f, 1.0f, 1.0f });
		s_Data.Shader->SetFloat3("dirLight.Specular",  { 1.0f,  1.0f,  1.0f });

		int lightCount = 0;
		for (PointLightComponent& light : scene->GetComponentGroup<PointLightComponent>())
		{
			if (lightCount >= 4) break;
			light.SetUniform(s_Data.Shader, lightCount);
			lightCount++;
		}
		s_Data.Shader->SetInt("u_PointLightCount", lightCount);

		for (MeshComponent& mesh : scene->GetComponentGroup<MeshComponent>())
		{
			DrawMesh(mesh);
		}


	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh)
	{
		s_Data.Shader->SetMat4("u_TransformMatrix", mesh->GetTransformMatrix());

		mesh->BindTexture(Utils::TextureType::DIFFUSE);
		mesh->BindTexture(Utils::TextureType::SPECULAR);
		Renderer3D::Flush(mesh);

	}

	void Renderer3D::Flush(const Ref<Mesh>& mesh)
	{
		ATL_PROFILE_FUNCTION();
		mesh->GetVertexArray()->BindAll();

		RenderCommand::DrawIndexed(mesh->GetVertexArray(), mesh->GetTriangleCount() * 3);
	}
}
