#include "atlpch.h"

#include "Renderer3D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

//TEMP:
#include "CubeMap.h"


namespace Atlas {

	static uint32_t MAX_POINT_LIGHTS = 128;

	struct LightVertex
	{
		glm::vec3 Position;
		int ID;
	};

	struct Renderer3DData
	{
		//TODO: Use shader lib
		Ref<Shader> MaterialShader;
		Ref<Shader> OutlineShader;
		Ref<Shader> CubemapShader;
		Ref<Shader>	DirLightDepthShader;
		Ref<Shader> NormalShader;
		Ref<Shader> LightShader;

		Ref<CubeMapTexture> SkyTexture;
		Ref<Texture2D> LightTexture;
		Ref<CubeMap> SkyCube;

		Ref<VertexArray> LightVertexArray;
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
		s_Data.SkyTexture->Bind((int)Utils::TextureType::SKYBOX);

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
		s_Data.LightShader = Shader::Create("assets/Shaders/LightShader.glsl");
		s_Data.MaterialShader->Bind();

		s_Data.SkyTexture = CubeMapTexture::Create({
			"assets/Textures/skybox/right.jpg",
			"assets/Textures/skybox/left.jpg",
			"assets/Textures/skybox/top.jpg",
			"assets/Textures/skybox/bottom.jpg",
			"assets/Textures/skybox/front.jpg",
			"assets/Textures/skybox/back.jpg"
			});
		s_Data.LightTexture = Texture2D::Create("assets/Textures/LightBulb.png");

		s_Data.SkyCube = CreateRef<CubeMap>();
		s_Data.SkyCube->Load();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(MAX_POINT_LIGHTS);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Int, "a_ID" },
			});

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(MAX_POINT_LIGHTS);

		s_Data.LightVertexArray = VertexArray::Create();
		s_Data.LightVertexArray->AddVertexBuffer(vertexBuffer);
		s_Data.LightVertexArray->SetIndexBuffer(indexBuffer);

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
				//uint32_t id = scene->HasComponent<IDComponent>(entity) ? scene->GetComponent<IDComponent>(entity) : -1;
				DrawMesh(*mesh->Mesh, entity);

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

			//TODO: find better way

			DrawMesh(mesh, selection);

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
		s_Data.SkyTexture->Bind();

		s_Data.CubemapShader->SetMat4("u_View", glm::mat4(glm::mat3(scene->getCamera().GetViewMatrix())));
		s_Data.CubemapShader->SetMat4("u_Projection", scene->getCamera().GetProjectionMatrix());
		Flush(s_Data.SkyCube->GetVertexArray(), 12);
		s_Data.MaterialShader->Bind();
		RenderCommand::SetDepthFunc(Utils::Operation::LESS);
		RenderCommand::Enalbe(Utils::Operation::STENCIL);

	}


	void Renderer3D::DrawLights(const Ref<Scene> scene)
	{
		std::vector<LightVertex> lightPositions;
		std::vector<uint32_t> lightIndices;

		ECS::Entity selection = scene->GetSelectedEntity();

		for (auto& entity : scene->GetComponentGroup<PointLightComponent>())
		{
			if (entity.EntityHandle != selection)
			{
				PointLightComponent* light = entity.Component;
				lightPositions.push_back({ light->Position, (int)entity.EntityHandle });
			}

		}

		for (uint32_t i = 0; i < lightPositions.size(); i++) lightIndices.push_back(i);

		s_Data.LightShader->Bind();
		s_Data.LightTexture->Bind();
		s_Data.LightShader->SetMat4("u_ViewMatrix", scene->getCamera().GetViewMatrix());
		s_Data.LightShader->SetMat4("u_ProjectionMatrix", scene->getCamera().GetProjectionMatrix());
		s_Data.LightShader->SetFloat3("u_ViewPosition", scene->getCamera().GetPosition());
		s_Data.LightShader->SetFloat4("u_TextureColor", { 1.0, 1.0, 1.0, 1.0 });

		if (lightPositions.size() != 0)
		{
			s_Data.LightVertexArray->GetVertexBuffer()->SetData(&lightPositions[0], lightPositions.size() * sizeof(LightVertex));
			s_Data.LightVertexArray->GetIndexBuffer()->SetData(&lightIndices[0], lightPositions.size() * sizeof(uint32_t));
			s_Data.LightVertexArray->BindAll();

			RenderCommand::DrawPoints(s_Data.LightVertexArray, lightIndices.size());
		}
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

	void Renderer3D::DrawOutline(const Ref<Scene> scene, const glm::vec4& color, float thickness)
	{
		ECS::Entity selection = scene->GetSelectedEntity();
		if (selection == ECS::null) return;

		if (scene->HasComponent<MeshComponent>(selection))
		{
			auto& mesh = scene->GetComponent<MeshComponent>(selection);
			s_Data.OutlineShader->Bind();
			s_Data.OutlineShader->SetFloat4("u_OutlineColor", color);
			s_Data.OutlineShader->SetFloat("u_Thickness", thickness);
			s_Data.OutlineShader->SetMat4("u_ViewProjection", scene->getCamera().GetViewProjectionMatrix());
			s_Data.OutlineShader->SetMat4("u_TransformMatrix", mesh.Mesh->GetTransformMatrix());

			RenderCommand::SetStencilFunc(Utils::Operation::NOTEQUAL, 1, 0xFF);
			RenderCommand::SetStencilMask(0x00);

			bool shading = mesh.Mesh->GetShading();
			mesh.Mesh->SetShading(true);
			Flush(mesh.Mesh->GetVertexArray(), mesh.Mesh->GetTriangleCount());
			mesh.Mesh->SetShading(shading);

			RenderCommand::SetStencilFunc(Utils::Operation::ALLWAYS, 1, 0xFF);
			RenderCommand::SetStencilMask(0xFF);
		}

		if (scene->HasComponent<PointLightComponent>(selection))
		{
			auto& light = scene->GetComponent<PointLightComponent>(selection);

			LightVertex vertex = { light.Position, selection };
			uint32_t index = 0;

			s_Data.LightVertexArray->GetVertexBuffer()->SetData(&vertex, sizeof(LightVertex));
			s_Data.LightVertexArray->GetIndexBuffer()->SetData(&index, sizeof(uint32_t));
			s_Data.LightVertexArray->BindAll();

			s_Data.LightShader->Bind();
			s_Data.LightTexture->Bind();
			s_Data.LightShader->SetMat4("u_ViewMatrix", scene->getCamera().GetViewMatrix());
			s_Data.LightShader->SetMat4("u_ProjectionMatrix", scene->getCamera().GetProjectionMatrix());
			s_Data.LightShader->SetFloat3("u_ViewPosition", scene->getCamera().GetPosition());

			s_Data.LightShader->SetFloat4("u_TextureColor", { 1.0, 0.0, 0.0, 1.0 });
			RenderCommand::DrawPoints(s_Data.LightVertexArray, 1);
		}

		s_Data.MaterialShader->Bind();
	}

	void Renderer3D::Flush(const Ref<VertexArray>& vertexArray, uint32_t triangleCount)
	{
		ATL_PROFILE_FUNCTION();
		vertexArray->BindAll();

		RenderCommand::DrawIndexed(vertexArray, triangleCount * 3);
	}

}
