#pragma once

#include <Atlas.h>


class Sandbox2D : public Atlas::Layer
{
private:
	Atlas::OrthographicCameraController m_CameraController;
	Atlas::Ref<Atlas::Texture2D> m_Texture;
	glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Atlas::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Atlas::Event& e) override;

};
