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
		//Mesh Object;
		glm::mat4 ProjMat = glm::mat4(0.0f);
		glm::mat4 RotMat = glm::mat4(1.0f);
		glm::mat4 ModelMat = glm::mat4(1.0f);
		glm::mat4 TransMat = glm::mat4(1.0f);
		glm::vec3 Camera = glm::vec3(0.0f);
		glm::vec3 BasicLightDir = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
		float Angle = 0.0f;
		Ref<Mesh> m_Mesh;
	};

	static Renderer3DData s_Data;

	void PrintMat4(glm::mat4& matrix)
	{
		std::string msg = "";
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				msg += std::to_string(matrix[i][j]) + ", ";
			}
			msg += "\n";
		}
		ATL_CORE_TRACE("matrix: \n {0}", msg);
	}

	void Renderer3D::Init()
	{
		ATL_PROFILE_FUNCTION();
		s_Data.m_Mesh = Mesh::Create("assets/Models/Teapot.obj");

		// camera class
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
		float fov = 90.0f;
		float aspectRatio = 1.0f; // should not be hard coded
		float fovRad = 1.0f / tanf(glm::radians(fov) / 2);

		s_Data.ProjMat = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		s_Data.TransMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -7.0f));

		s_Data.Shader = Atlas::Shader::Create("assets/Shaders/Material.glsl");
		s_Data.Shader->Bind();
	}

	void Renderer3D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();
	}

	void Renderer3D::BeginScene()
	{
		ATL_PROFILE_FUNCTION();
		//TODO: 
		s_Data.Shader->Bind();

		s_Data.RotMat = glm::rotate(glm::mat4(1.0f), glm::radians(s_Data.Angle), glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.Angle += 0.5f;

		s_Data.Shader->SetMat4("u_ViewProjection", s_Data.ProjMat);
		s_Data.Shader->SetMat4("u_RotationMatrix", s_Data.RotMat);
		s_Data.Shader->SetMat4("u_TranslationMatrix", s_Data.TransMat);
		s_Data.Shader->SetFloat3("u_LightDir", s_Data.BasicLightDir);

	}

	void Renderer3D::EndScene()
	{
		ATL_PROFILE_FUNCTION();
		Flush();
	}

	void Renderer3D::Flush()
	{
		ATL_PROFILE_FUNCTION();
		s_Data.m_Mesh->GetVertexArray()->BindAll();

		RenderCommand::DrawIndexed(s_Data.m_Mesh->GetVertexArray(), s_Data.m_Mesh->GetTriangleCount() * 3);
	}
}
