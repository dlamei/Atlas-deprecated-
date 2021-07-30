#include <Atlas.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Atlas::Layer
{
private:
	std::shared_ptr<Atlas::Shader> m_Shader;
	std::shared_ptr<Atlas::VertexArray> m_VertexArray;

	std::shared_ptr<Atlas::Shader> m_SquareShader;
	std::shared_ptr<Atlas::VertexArray> m_SquareVertexArray;

	Atlas::OrthographicCamera m_Camera;


public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Atlas::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Atlas::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Atlas::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{ Atlas::ShaderDataType::Float3, "a_Position" },
			{ Atlas::ShaderDataType::Float4, "a_Color" }
			});

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<Atlas::IndexBuffer> indexBuffer;
		indexBuffer.reset(Atlas::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Atlas::Shader(vertexSrc, fragmentSrc));

		float squareVertices[4 * 3] = {
			-0.6f, -0.6f, 0.0f,
			 0.6f,  0.6f, 0.0f,
			-0.6f,  0.6f, 0.0f,
			 0.6f, -0.6f, 0.0f
		};

		std::shared_ptr<Atlas::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Atlas::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{ Atlas::ShaderDataType::Float3, "a_Position" },
			});

		m_SquareVertexArray.reset(Atlas::VertexArray::Create());
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int squareIndices[] = { 0, 1, 2, 0, 3, 1 };

		std::shared_ptr<Atlas::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Atlas::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string squareVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = vec4(v_Position, 1.0);
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;


			void main()
			{
				color = vec4(0.3, 0.0, 0.2, 1.0);
			}
		)";

		m_SquareShader.reset(new Atlas::Shader(squareVertexSrc, squareFragmentSrc));

	}

	void OnUpdate() override
	{
		Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Atlas::RenderCommand::Clear();

		m_Camera.SetRotation(45.0f);

		Atlas::Renderer::BeginScene(m_Camera);

		Atlas::Renderer::Submit(m_SquareShader, m_SquareVertexArray);
		Atlas::Renderer::Submit(m_Shader, m_VertexArray);

		Atlas::Renderer::EndScene();
	}

	void OnEvent(Atlas::Event& event) override
	{
	}

	void OnImGuiRender() override
	{
	}
};

class Sandbox : public Atlas::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Atlas::Application* Atlas::CreateApplication()
{
	return new Sandbox();
}
