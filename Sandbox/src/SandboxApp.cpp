#include <Atlas.h>

#include "ImGui/imgui.h"


#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Atlas::Layer
{
private:
	std::shared_ptr<Atlas::Shader> m_Shader;
	std::shared_ptr<Atlas::VertexArray> m_VertexArray;

	std::shared_ptr<Atlas::Shader> m_SquareShader;
	std::shared_ptr<Atlas::VertexArray> m_SquareVertexArray;

	Atlas::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;

	float m_CameraSpeed = 4.0f;
	float m_ZoomSpeed = 10.0f;
	float m_CameraRotationSpeed = 180.0f;

	Atlas::Timestep timestep = 0.0f;

public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 0.0f) 
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

		m_Shader.reset(new Atlas::Shader(vertexSrc, fragmentSrc));

		float squareVertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
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
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = vec4(v_Position, 1.0);
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

	void OnUpdate(Atlas::Timestep ts) override
	{
		timestep = ts;

		if (Atlas::Input::IsKeyPressed(ATL_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed * timestep;
		}
		else if (Atlas::Input::IsKeyPressed(ATL_KEY_A))
		{
			m_CameraPosition.x -= m_CameraSpeed * timestep;
		}

		if (Atlas::Input::IsKeyPressed(ATL_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed * timestep;
		}
		else if (Atlas::Input::IsKeyPressed(ATL_KEY_S))
		{
			m_CameraPosition.y -= m_CameraSpeed * timestep;
		}

		if (Atlas::Input::IsKeyPressed(ATL_KEY_Q))
		{
			m_CameraRotation -= m_CameraRotationSpeed * timestep;
		}
		else if (Atlas::Input::IsKeyPressed(ATL_KEY_E))
		{
			m_CameraRotation += m_CameraRotationSpeed * timestep;
		}

		Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Atlas::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Atlas::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.15f, j * 0.15f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Atlas::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
			}
		}
		Atlas::Renderer::Submit(m_Shader, m_VertexArray);

		Atlas::Renderer::EndScene();
	}

	void OnEvent(Atlas::Event& event) override
	{
		if (event.GetEventType() == Atlas::EventType::MouseScrolled)
		{
			Atlas::MouseScrolledEvent e = (Atlas::MouseScrolledEvent&)event;
			if (e.GetYOffset() == 1)
			{
				m_Camera.SetZoom(m_Camera.GetZoom() - m_ZoomSpeed * timestep);
			}
			else 
			{
				m_Camera.SetZoom(m_Camera.GetZoom() + m_ZoomSpeed * timestep);
			}

		}
	}

	void OnImGuiRender() override
	{
		ImGui::ShowDemoWindow();
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
