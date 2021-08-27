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
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		Ref<Shader> Shader;

		uint32_t IndexCount = 0;
		uint32_t VertexCount = 0;

		//TEMP
		glm::vec3 BasicLightDir = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
		float Angle = 0.0f;

		Ref<Mesh> m_Mesh;
		PerspectiveCamera Camera = PerspectiveCamera(0.1f, 1000.0f, glm::radians(90.0f), 1.0f);

	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		ATL_PROFILE_FUNCTION();
		s_Data.m_Mesh = Mesh::Create("assets/Models/Box.obj");

		s_Data.m_Mesh->SetTranslation(glm::vec3(0.0f, 0.0f, -1.0f));
		s_Data.m_Mesh->AddTexture(Texture2D::Create("assets/Textures/Box.png"), Utils::TextureType::DIFFUSE);
		s_Data.m_Mesh->AddTexture(Texture2D::Create("assets/Textures/Box_Specular.png"), Utils::TextureType::SPECULAR);

		s_Data.Shader = Atlas::Shader::Create("assets/Shaders/Material.glsl");
		s_Data.Shader->Bind();
	}

	void Renderer3D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();
	}

	//void Renderer3D::BeginScene(const PerspectiveCamera& camera, bool shading)
	void Renderer3D::DrawScene(const Ref<Scene> scene)
	{
		ATL_PROFILE_FUNCTION();
		s_Data.Shader->Bind();
		//s_Data.m_Mesh->BindTexture(Utils::TextureType::DIFFUSE);
		//s_Data.m_Mesh->BindTexture(Utils::TextureType::SPECULAR);
		scene->GetMesh()->BindTexture(Utils::TextureType::DIFFUSE);
		scene->GetMesh()->BindTexture(Utils::TextureType::SPECULAR);

		//s_Data.m_Mesh->SetShading(shading);

		s_Data.m_Mesh->SetRotation(glm::vec3(1.0f), s_Data.Angle);
		s_Data.Angle += 0.5f;

		//s_Data.Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data.Shader->SetMat4("u_ViewProjection", scene->getCamera().GetViewProjectionMatrix());
		s_Data.Shader->SetMat4("u_RotationMatrix", glm::mat4(1.0f));
		s_Data.Shader->SetMat4("u_TranslationMatrix", s_Data.m_Mesh->GetTranslationMatrix());

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

		Flush(scene);

	}

	void Renderer3D::EndScene()
	{
		ATL_PROFILE_FUNCTION();
		//Flush();
	}

	void Renderer3D::Flush(const Ref<Scene> scene)
	{
		ATL_PROFILE_FUNCTION();
		//s_Data.m_Mesh->GetVertexArray()->BindAll();
		scene->GetMesh()->GetVertexArray()->BindAll();

		RenderCommand::DrawIndexed(scene->GetMesh()->GetVertexArray(), scene->GetMesh()->GetTriangleCount() * 3);
		//RenderCommand::DrawIndexedWireframe(scene->GetMesh()->GetVertexArray(), scene->GetMesh()->GetTriangleCount() * 3);
	}
}
