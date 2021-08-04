#include <Atlas.h>
#include <Atlas/Core/EntryPoint.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Atlas::Layer
{
private:
	Atlas::ShaderLibrary m_ShaderLibrary;
	Atlas::Ref<Atlas::VertexArray> m_VertexArray;

	Atlas::Ref<Atlas::VertexArray> m_SquareVertexArray;

	Atlas::Ref<Atlas::Texture2D> m_Texture;
	Atlas::Ref<Atlas::Texture2D> m_CatTexture;

	Atlas::OrthographicCameraController m_CameraController;

	glm::vec4 m_Color1 = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_Color2 = { 0.2f, 0.3f, 0.8f, 1.0f };

public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = Atlas::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		Atlas::Ref<Atlas::VertexBuffer> vertexBuffer;
		vertexBuffer = Atlas::VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ Atlas::ShaderDataType::Float3, "a_Position" },
			{ Atlas::ShaderDataType::Float4, "a_Color" }
			});

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		Atlas::Ref<Atlas::IndexBuffer> indexBuffer;
		indexBuffer = Atlas::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_ShaderLibrary.Add(Atlas::Shader::Create("TriangleShader", vertexSrc, fragmentSrc));

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Atlas::Ref<Atlas::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = Atlas::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVertexBuffer->SetLayout({
			{ Atlas::ShaderDataType::Float3, "a_Position" },
			{ Atlas::ShaderDataType::Float2, "a_TexCoord" },
			});

		m_SquareVertexArray = Atlas::VertexArray::Create();
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };

		Atlas::Ref<Atlas::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = Atlas::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		m_ShaderLibrary.Load("assets/Shaders/FlatColor.glsl");

		auto textureShader = m_ShaderLibrary.Load("assets/Shaders/Texture.glsl");

		m_CatTexture = Atlas::Texture2D::Create("assets/Textures/cat.png");
		m_Texture = Atlas::Texture2D::Create("assets/Textures/colorgrid.png");

		m_Texture->Bind();

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);


	}

	void OnUpdate(Atlas::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Atlas::RenderCommand::Clear();

		Atlas::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		//Atlas::Renderer::Submit(m_Shader, m_VertexArray);

		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.12f, j * 0.12f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				flatColorShader->Bind();
				if ((i + j) % 2 == 0)
				{
					flatColorShader->SetFloat4("u_Color", m_Color1);
				}
				else
				{
					flatColorShader->SetFloat4("u_Color", m_Color2);
				}

				Atlas::Renderer::Submit(flatColorShader, m_SquareVertexArray, transform);

				textureShader->Bind();
				m_CatTexture->Bind();
				Atlas::Renderer::Submit(textureShader, m_SquareVertexArray, transform);
			}
		}

		m_Texture->Bind();
		Atlas::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Atlas::Renderer::EndScene();
	}

	void OnEvent(Atlas::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::SetWindowFontScale(1.8f);
		ImGui::ColorEdit3("color1", glm::value_ptr(m_Color1));
		ImGui::ColorEdit3("color2", glm::value_ptr(m_Color2));
		ImGui::End();
	}
};

class Sandbox : public Atlas::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Atlas::Application* Atlas::CreateApplication()
{
	return new Sandbox();
}
