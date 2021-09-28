#include "atlpch.h"

#include "Renderer3D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

//TEMP
#include <glad/glad.h>

namespace Atlas {

	struct Renderer3DData
	{
		Ref<Shader> MaterialShader;
		Ref<Shader> OutlineShader;
	};

	static Renderer3DData s_Data;

	void Renderer3D::DrawMesh(Mesh& mesh, const uint32_t id)
	{
		s_Data.MaterialShader->SetFloat("material.Shininess", 8.0f);
		s_Data.MaterialShader->SetMat4("u_TransformMatrix", mesh.GetTransformMatrix());
		s_Data.MaterialShader->SetInt("u_DisplayMode", (int)mesh.GetDisplayMode());
		s_Data.MaterialShader->SetInt("u_ID", id);
		mesh.BindTexture(Utils::TextureType::DIFFUSE);

		Flush(mesh);
	}

	void Renderer3D::Init()
	{
		ATL_PROFILE_FUNCTION();

		s_Data.MaterialShader = Shader::Create("assets/Shaders/Material.glsl");
		s_Data.OutlineShader = Shader::Create("assets/Shaders/Outline.glsl");
		s_Data.MaterialShader->Bind();
	}

	void Renderer3D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawScene(const Ref<Scene> scene)
	{
		ATL_PROFILE_FUNCTION();

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0x00);


		s_Data.MaterialShader->Bind();

		s_Data.MaterialShader->SetMat4("u_ViewProjection", scene->getCamera().GetViewProjectionMatrix());
		s_Data.MaterialShader->SetFloat3("u_ViewPosition", scene->getCamera().GetPosition());

		s_Data.MaterialShader->SetInt("material.DiffuseTexture", (int)Utils::TextureType::DIFFUSE);
		s_Data.MaterialShader->SetInt("material.SpecularTexture", (int)Utils::TextureType::SPECULAR);


		int dirLightCount = 0;
		for (DirLightComponent& light : scene->GetComponentGroup<DirLightComponent>())
		{
			if (dirLightCount >= 4) break;
			light.SetUniform(s_Data.MaterialShader, dirLightCount);
			dirLightCount++;
		}
		s_Data.MaterialShader->SetInt("u_DirLightCount", dirLightCount);

		int pointLightCount = 0;
		for (PointLightComponent& light : scene->GetComponentGroup<PointLightComponent>())
		{
			if (pointLightCount >= 4) break;
			light.SetUniform(s_Data.MaterialShader, pointLightCount);
			pointLightCount++;
		}
		s_Data.MaterialShader->SetInt("u_PointLightCount", pointLightCount);

		for (auto& entity : scene->GetComponentGroup<MeshComponent>())
		{
			auto& mesh = entity.Component;

			if (!mesh->Hide && entity.EntityHandle != scene->GetSelectedEntity())
			{
				uint32_t id = scene->HasComponent<IDComponent>(entity) ? scene->GetComponent<IDComponent>(entity) : -1;
				DrawMesh(*mesh->Mesh, id);
			}
		}

		ECS::Entity selection = scene->GetSelectedEntity();
		if (selection != ECS::null && scene->HasComponent<MeshComponent>(selection))
		{
			MeshComponent& mesh = scene->GetComponent<MeshComponent>(selection);
			glStencilMask(0xff);
			uint32_t id = scene->HasComponent<IDComponent>(selection) ? scene->GetComponent<IDComponent>(selection) : -1;
			DrawMesh(mesh, id);
		}


	}

	void Renderer3D::Flush(const Mesh& mesh)
	{
		ATL_PROFILE_FUNCTION();
		mesh.GetVertexArray()->BindAll();

		RenderCommand::DrawIndexed(mesh.GetVertexArray(), mesh.GetTriangleCount() * 3);
	}

	void Renderer3D::DrawOutline(const Ref<Mesh>& mesh, const glm::mat4& viewProjMat, const glm::vec4& color, float thickness)
	{
		s_Data.OutlineShader->Bind();
		s_Data.OutlineShader->SetFloat4("u_OutlineColor", color);
		s_Data.OutlineShader->SetFloat("u_Thickness", thickness);
		s_Data.OutlineShader->SetMat4("u_ViewProjection", viewProjMat);
		s_Data.OutlineShader->SetMat4("u_TransformMatrix", mesh->GetTransformMatrix());

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		Flush(*mesh);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		s_Data.MaterialShader->Bind();
	}
}
