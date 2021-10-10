#pragma once

#include <Atlas.h>

class Sandbox2D : public Atlas::Layer
{
private:
	Atlas::OrthographicCameraController m_Controller;

public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Atlas::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Atlas::Event& e) override;

};
