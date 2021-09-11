#pragma once

#include <Atlas.h>

class Sandbox3D : public Atlas::Layer
{
private:
	Atlas::PerspectiveCameraController m_CameraController;
	bool m_Shading = true;

	Atlas::Ref<Atlas::FrameBuffer> m_FrameBuffer;
	uint32_t m_ColorAttachment = 0;
	uint32_t m_DepthAttachment = 0;
	glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	Sandbox3D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Atlas::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Atlas::Event& e) override;

};
