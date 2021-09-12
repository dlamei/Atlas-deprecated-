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

		s_Data.Shader->SetInt("material.DiffuseTexture", Utils::TextureType::DIFFUSE);
		s_Data.Shader->SetInt("material.SpecularTexture", Utils::TextureType::SPECULAR);

		s_Data.Shader->SetFloat3("material.AmbientColor", { 0.6f, 0.0f, 0.3f });
		s_Data.Shader->SetFloat3("material.DiffuseColor", { 0.6f, 0.0f, 0.3f });
		s_Data.Shader->SetFloat3("material.SpecularColor", { 0.5f, 0.5f, 0.5f });
		s_Data.Shader->SetFloat("material.Shininess", 16.0f);

		s_Data.Shader->SetFloat3("light.Position", glm::vec3(0.0f, 4.0f, -3.0f));
		s_Data.Shader->SetFloat3("light.AmbientColor", { 0.4f, 0.4f, 0.4f });
		s_Data.Shader->SetFloat3("light.DiffuseColor", { 0.5f, 0.5f, 0.5f });
		s_Data.Shader->SetFloat3("light.SpecularColor", { 1.0f, 1.0f, 1.0f });

		for (MeshComponent& mesh : scene->GetComponentGroup<MeshComponent>())
		{
			DrawMesh(mesh);
		}


	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh)
	{
		s_Data.Shader->SetMat4("u_RotationMatrix", mesh->GetRotationMatrix());
		s_Data.Shader->SetMat4("u_TranslationMatrix", mesh->GetTranslationMatrix());

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
