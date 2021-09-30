#pragma once

#include "Atlas/Renderer/RendererAPI.h"

namespace Atlas {

	class OpenGLRendererAPI : public RendererAPI
	{
	private:

	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void Enable(Utils::Operation operation) override;
		virtual void Disable(Utils::Operation operation) override;

		virtual void SetStencilOp(Utils::Operation op1, Utils::Operation op2, Utils::Operation op3) override;
		virtual void SetStencilFunc(Utils::Operation operation, int ref, uint32_t mask) override;
		virtual void SetStencilMask(uint32_t mask) override;

		virtual void SetDepthFunc(Utils::Operation operation) override;

		virtual void DrawIndexedWireframe(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) override;

		virtual void Bind2DTexture(uint32_t id) override;
	};

}
