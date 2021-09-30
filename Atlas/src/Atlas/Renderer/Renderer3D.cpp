#include "atlpch.h"

#include "Renderer3D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include "CubeMap.h"

//TEMP
#include <glad/glad.h>


namespace Atlas {

	struct Renderer3DData
	{
		//TODO: Use shader lib
		Ref<Shader> MaterialShader;
		Ref<Shader> OutlineShader;
		Ref<Shader> CubemapShader;
		Ref<Shader>	DirLightDepthShader;
		Ref<Shader> NormalShader;

		Ref<Texture> CubeTexture;
		Ref<CubeMap> SkyCube;
	};

	static Renderer3DData s_Data;

	//TODO: material system
	void Renderer3D::DrawMesh(Mesh& mesh, const uint32_t id)
	{
		ATL_PROFILE_FUNCTION();

		s_Data.MaterialShader->SetFloat("material.Shininess", 8.0f);
		s_Data.MaterialShader->SetMat4("u_TransformMatrix", mesh.GetTransformMatrix());
		s_Data.MaterialShader->SetInt("u_ID", id);
		mesh.BindTexture(Utils::TextureType::DIFFUSE);
		mesh.BindTexture(Utils::TextureType::SPECULAR);
		s_Data.CubeTexture->Bind((int)Utils::TextureType::SKYBOX);

		Flush(mesh.GetVertexArray(), mesh.GetTriangleCount());
	}

	void Renderer3D::Init()
	{
		ATL_PROFILE_FUNCTION();

		{
			using namespace Utils;
			RenderCommand::Enalbe(Operation::DEPTH);
			RenderCommand::SetDepthFunc(Operation::LESS);
			RenderCommand::Enalbe(Operation::STENCIL);
			RenderCommand::Enalbe(Operation::CULL_FACES);
			RenderCommand::SetStencilOp(Operation::KEEP, Operation::KEEP, Operation::REPLACE);
		}

		s_Data.MaterialShader = Shader::Create("assets/Shaders/Material.glsl");
		s_Data.OutlineShader = Shader::Create("assets/Shaders/Outline.glsl");
		s_Data.CubemapShader = Shader::Create("assets/Shaders/SkyBox.glsl");
		s_Data.DirLightDepthShader = Shader::Create("assets/Shaders/DirLightDepthMap.glsl");
		s_Data.NormalShader = Shader::Create("assets/Shaders/NormalShader.glsl");
		s_Data.MaterialShader->Bind();

		s_Data.CubeTexture = CubeMapTexture::Create({
			"assets/Textures/skybox/right.jpg",
			"assets/Textures/skybox/left.jpg",
			"assets/Textures/skybox/top.jpg",
			"assets/Textures/skybox/bottom.jpg",
			"assets/Textures/skybox/front.jpg",
			"assets/Textures/skybox/back.jpg"
			});

		s_Data.SkyCube = CreateRef<CubeMap>();
		s_Data.SkyCube->Load();

	}

	void Renderer3D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawScene(const Ref<Scene> scene)
	{
		ATL_PROFILE_FUNCTION();

		RenderCommand::SetStencilFunc(Utils::Operation::ALLWAYS, 1, 0xff);
		RenderCommand::SetStencilMask(0x00);


		s_Data.MaterialShader->Bind();

		s_Data.MaterialShader->SetMat4("u_ViewProjection", scene->getCamera().GetViewProjectionMatrix());
		s_Data.MaterialShader->SetFloat3("u_ViewPosition", scene->getCamera().GetPosition());

		s_Data.MaterialShader->SetInt("material.DiffuseTexture", (int)Utils::TextureType::DIFFUSE);
		s_Data.MaterialShader->SetInt("material.SpecularTexture", (int)Utils::TextureType::SPECULAR);
		s_Data.MaterialShader->SetInt("u_SkyCubeTexture", (int)Utils::TextureType::SKYBOX);


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

				if (mesh->Mesh->GetDisplayMode() == Utils::DisplayMode::NORMAL)
				{
					s_Data.NormalShader->Bind();
					s_Data.NormalShader->SetMat4("u_ViewMatrix", scene->getCamera().GetViewMatrix());
					s_Data.NormalShader->SetMat4("u_ProjectionMatrix", scene->getCamera().GetProjectionMatrix());
					s_Data.NormalShader->SetMat4("u_ModelMatrix", mesh->Mesh->GetTransformMatrix());
					Flush(mesh->Mesh->GetVertexArray(), mesh->Mesh->GetTriangleCount());
					s_Data.MaterialShader->Bind();
				}
			}
		}

		ECS::Entity selection = scene->GetSelectedEntity();
		if (selection != ECS::null && scene->HasComponent<MeshComponent>(selection))
		{
			MeshComponent& mesh = scene->GetComponent<MeshComponent>(selection);
			RenderCommand::SetStencilMask(0xFF);
			uint32_t id = scene->HasComponent<IDComponent>(selection) ? scene->GetComponent<IDComponent>(selection) : -1;

			//TODO: find better way

			DrawMesh(mesh, id);

			if (mesh.Mesh->GetDisplayMode() == Utils::DisplayMode::NORMAL)
			{
				s_Data.NormalShader->Bind();
				s_Data.NormalShader->SetMat4("u_ViewMatrix", scene->getCamera().GetViewMatrix());
				s_Data.NormalShader->SetMat4("u_ProjectionMatrix", scene->getCamera().GetProjectionMatrix());
				s_Data.NormalShader->SetMat4("u_ModelMatrix", mesh.Mesh->GetTransformMatrix());
				Flush(mesh.Mesh->GetVertexArray(), mesh.Mesh->GetTriangleCount());
				s_Data.MaterialShader->Bind();
			}
		}


		//Draw Sky Texture
		RenderCommand::Disable(Utils::Operation::STENCIL);
		RenderCommand::SetDepthFunc(Utils::Operation::LEQUAL);
		s_Data.CubemapShader->Bind();
		s_Data.CubeTexture->Bind();

		s_Data.CubemapShader->SetMat4("u_View", glm::mat4(glm::mat3(scene->getCamera().GetViewMatrix())));
		s_Data.CubemapShader->SetMat4("u_Projection", scene->getCamera().GetProjectionMatrix());
		Flush(s_Data.SkyCube->GetVertexArray(), 12);
		s_Data.MaterialShader->Bind();
		RenderCommand::SetDepthFunc(Utils::Operation::LESS);
		RenderCommand::Enalbe(Utils::Operation::STENCIL);

	}

	void Renderer3D::DrawLightDepthMap(Ref<Scene> scene, const glm::mat4& viewProjection)
	{
		s_Data.DirLightDepthShader->Bind();
		s_Data.DirLightDepthShader->SetMat4("u_ViewProjection", viewProjection);

		for (MeshComponent& mesh : scene->GetComponentGroup<MeshComponent>())
		{
			s_Data.DirLightDepthShader->SetMat4("u_ModelMat", mesh.Mesh->GetTransformMatrix());
			Flush(mesh.Mesh->GetVertexArray(), mesh.Mesh->GetTriangleCount());
		}
	}

	void Renderer3D::Flush(const Ref<VertexArray>& vertexArray, uint32_t triangleCount)
	{
		ATL_PROFILE_FUNCTION();
		vertexArray->BindAll();

		RenderCommand::DrawIndexed(vertexArray, triangleCount * 3);
	}

	void Renderer3D::DrawOutline(const Ref<Mesh>& mesh, const glm::mat4& viewProjMat, const glm::vec4& color, float thickness)
	{
		s_Data.OutlineShader->Bind();
		s_Data.OutlineShader->SetFloat4("u_OutlineColor", color);
		s_Data.OutlineShader->SetFloat("u_Thickness", thickness);
		s_Data.OutlineShader->SetMat4("u_ViewProjection", viewProjMat);
		s_Data.OutlineShader->SetMat4("u_TransformMatrix", mesh->GetTransformMatrix());

		RenderCommand::SetStencilFunc(Utils::Operation::NOTEQUAL, 1, 0xFF);
		RenderCommand::SetStencilMask(0x00);

		bool shading = mesh->GetShading();
		mesh->SetShading(true);
		Flush(mesh->GetVertexArray(), mesh->GetTriangleCount());
		mesh->SetShading(shading);

		RenderCommand::SetStencilFunc(Utils::Operation::ALLWAYS, 1, 0xFF);
		RenderCommand::SetStencilMask(0xFF);

		s_Data.MaterialShader->Bind();
	}
}
