#include "atlpch.h"

#include "Renderer3D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Atlas {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Color;
	};

	struct Triangle
	{
		glm::vec3 Points[3];
	};

	// should be a mesh class
	struct Mesh
	{
		std::vector<Triangle> Triangles;

		bool Load(const std::string filename)
		{
			ATL_PROFILE_FUNCTION();
			std::ifstream file(filename);

			if (!file.is_open()) return false;

			std::vector<glm::vec3> verticies;

			while (!file.eof())
			{
				//TODO: check if ok
				char line[128];
				file.getline(line, 128);

				std::stringstream s;
				s << line;

				char c;
				if (line[0] == 'v')
				{
					glm::vec3 vec;
					s >> c >> vec.x >> vec.y >> vec.z;
					verticies.push_back(vec);
				}
				else if (line[0] == 'f')
				{
					int indx[3];
					s >> c >> indx[0] >> indx[1] >> indx[2];
					Triangles.push_back({ verticies[indx[0] - 1], verticies[indx[1] - 1], verticies[indx[2] - 1] });
				}
			}
			return true;
		}
	};

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

		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;

		uint32_t* IndexBufferBase = nullptr;
		uint32_t* IndexBufferPtr = nullptr;

		//TEMP
		Mesh Object;
		glm::mat4 ProjMat = glm::mat4(0.0f);
		glm::mat4 RotMat = glm::mat4(1.0f);
		glm::mat4 ModelMat = glm::mat4(1.0f);
		glm::mat4 TransMat = glm::mat4(1.0f);
		glm::vec3 Camera = glm::vec3(0.0f);
		glm::vec3 BasicLightDir = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
		float Angle = 0.0f;
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
		s_Data.Object.Load("assets/Models/Hand.obj");
		ATL_PROFILE_FUNCTION();
		/*
		s_Data.Object.Triangles = {
			{glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.0f) },
			{glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 0.0f, 0.0f) },

			{glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 1.0f) },
			{glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 0.0f, 1.0f) },

			{glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 1.0f) },
			{glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 1.0f) },

			{glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 0.0f) },
			{glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(0.0f, 0.0f, 0.0f) },

			{glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(0.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, 1.0f) },
			{glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, 0.0f) },

			{glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 0.0f) },
			{glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 0.0f, 0.0f) },
		};
		*/

		// camera class
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
		float fov = 90.0f;
		float aspectRatio = 1.0f; // should not be hard coded
		float fovRad = 1.0f / tanf(glm::radians(fov) / 2);

		s_Data.ProjMat = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		s_Data.TransMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -7.0f));

		s_Data.VertexArray = VertexArray::Create();

		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex));
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float4, "a_Color" }
			});

		s_Data.IndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);

		s_Data.VertexBufferBase = new Vertex[s_Data.MaxVertices];
		s_Data.IndexBufferBase = new uint32_t[s_Data.MaxIndices];

		s_Data.Shader = Atlas::Shader::Create("assets/Shaders/Material.glsl");
		s_Data.Shader->Bind();

		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);
		s_Data.VertexArray->SetIndexBuffer(s_Data.IndexBuffer);

	}

	//TODO: remove
	void Renderer3D::OnUpdate()
	{
		ATL_PROFILE_FUNCTION();

		for (auto& tri : s_Data.Object.Triangles)
		{
			glm::vec3 v1 = tri.Points[1] - tri.Points[0];
			glm::vec3 v2 = tri.Points[2] - tri.Points[0];

			glm::vec3 normal = s_Data.RotMat * glm::vec4(glm::normalize(glm::cross(v1, v2)), 1.0f);
			glm::vec3 point = s_Data.ModelMat * glm::vec4(tri.Points[0], 1.0f);

			if (glm::dot(normal, point - s_Data.Camera) > 0) continue;

			float brightness = glm::dot(normal, s_Data.BasicLightDir);
			glm::vec4 color = glm::vec4(brightness, brightness, brightness, 1.0f);

			s_Data.VertexBufferPtr->Position = tri.Points[0];
			s_Data.VertexBufferPtr->Normal = normal;
			s_Data.VertexBufferPtr->Color = color;
			s_Data.VertexBufferPtr++;
			s_Data.VertexBufferPtr->Position = tri.Points[1];
			s_Data.VertexBufferPtr->Normal = normal;
			s_Data.VertexBufferPtr->Color = color;
			s_Data.VertexBufferPtr++;
			s_Data.VertexBufferPtr->Position = tri.Points[2];
			s_Data.VertexBufferPtr->Normal = normal;
			s_Data.VertexBufferPtr->Color = color;
			s_Data.VertexBufferPtr++;

			*s_Data.IndexBufferPtr = s_Data.VertexCount + 0;
			s_Data.IndexBufferPtr++;
			*s_Data.IndexBufferPtr = s_Data.VertexCount + 1;
			s_Data.IndexBufferPtr++;
			*s_Data.IndexBufferPtr = s_Data.VertexCount + 2;
			s_Data.IndexBufferPtr++;

			s_Data.IndexCount += 3;
			s_Data.VertexCount += 3;
		}
	}

	void Renderer3D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();

		delete[] s_Data.VertexBufferBase;
		delete s_Data.VertexBufferPtr;

		delete[] s_Data.IndexBufferBase;
		delete s_Data.IndexBufferPtr;
	}

	void Renderer3D::BeginScene()
	{
		ATL_PROFILE_FUNCTION();
		//TODO: 
		s_Data.VertexArray->BindAll();

		s_Data.Shader->Bind();

		s_Data.RotMat = glm::rotate(glm::mat4(1.0f), glm::radians(s_Data.Angle), glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.Angle += 0.5f;

		s_Data.ModelMat = s_Data.TransMat * s_Data.RotMat;
		s_Data.Shader->SetMat4("u_ViewProjection", s_Data.ProjMat);
		s_Data.Shader->SetMat4("u_ModelProjection", s_Data.TransMat * s_Data.RotMat);


		s_Data.IndexCount = 0;
		s_Data.VertexCount = 0;

		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		s_Data.IndexBufferPtr = s_Data.IndexBufferBase;
	}

	void Renderer3D::EndScene()
	{
		ATL_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
		s_Data.VertexBuffer->SetData(s_Data.VertexBufferBase, dataSize);

		dataSize = (uint32_t)((uint8_t*)s_Data.IndexBufferPtr - (uint8_t*)s_Data.IndexBufferBase);
		s_Data.IndexBuffer->SetData(s_Data.IndexBufferBase, dataSize);

		Flush();
	}

	void Renderer3D::Flush()
	{
		ATL_PROFILE_FUNCTION();

		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.IndexCount);
	}
}
