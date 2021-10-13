#include "Sandbox2D.h"

using namespace Atlas;

Sandbox2D::Sandbox2D()
	: m_Controller(16.0f / 9.0f)
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Atlas::Timestep ts)
{
	m_Controller.OnUpdate(ts);

	uint32_t size = 100;

	Renderer2D::BeginScene(m_Controller.GetCamera());


	RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	RenderCommand::Clear();

	for (uint32_t i = 0; i < size; i++)
	{
		for (uint32_t j = 0; j < size; j++)
		{
			Renderer2D::DrawRect({ i, j }, { 0.8, 0.8 }, { (float) i / size, (float) j / size, 0.0f, 1.0f });
		}
	}

	Renderer2D::EndScene();

	//fb->Unbind();
}

void Sandbox2D::OnImGuiRender()
{
}

void Sandbox2D::OnEvent(Atlas::Event& e)
{
	m_Controller.OnEvent(e);
}
