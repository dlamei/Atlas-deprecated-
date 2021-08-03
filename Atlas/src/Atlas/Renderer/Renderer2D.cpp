#include "atlpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "OrthographicCamera.h"

namespace Atlas {

	struct Renderer2DStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->VertexArray = VertexArray::Create();

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		s_Data->VertexArray->AddVertexBuffer(vertexBuffer);
		s_Data->VertexArray->SetIndexBuffer(indexBuffer);

		s_Data->Shader = Atlas::Shader::Create("assets/Shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->SetUniformMat4("u_Transform", glm::mat4(1.0f));

	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{		
		DrawRect({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{		
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->SetUnifromFloat4("u_Color", color);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);

	}
}
