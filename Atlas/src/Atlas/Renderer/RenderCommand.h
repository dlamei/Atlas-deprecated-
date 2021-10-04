#pragma once

#include "RendererAPI.h"

namespace Atlas {

	class RenderCommand
	{
	private:
		static RendererAPI* s_RendererAPI;

	public:
		inline static void Init() { s_RendererAPI->Init(); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void Enalbe(Atlas::Utils::Operation operation) { s_RendererAPI->Enable(operation); }
		inline static void Disable(Atlas::Utils::Operation operation) { s_RendererAPI->Disable(operation); }
		inline static void SetStencilOp(Utils::Operation op1, Utils::Operation op2, Utils::Operation op3) { s_RendererAPI->SetStencilOp(op1, op2, op3); }
		inline static void SetStencilFunc(Utils::Operation operation, int ref, uint32_t mask) { s_RendererAPI->SetStencilFunc(operation, ref, mask); }
		inline static void SetStencilMask(uint32_t mask) { s_RendererAPI->SetStencilMask(mask); }
		inline static void SetDepthFunc(Utils::Operation operation) { s_RendererAPI->SetDepthFunc(operation); }
		inline static void SetCull(Utils::Operation op) { s_RendererAPI->SetCull(op); }
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexSize = 0) { s_RendererAPI->DrawIndexed(vertexArray, indexSize); }
		inline static void DrawPoints(const Ref<VertexArray>& vertexArray, const uint32_t indexSize = 0) { s_RendererAPI->DrawPoints(vertexArray, indexSize); }
		inline static void DrawIndexedWireframe(const Ref<VertexArray>& vertexArray, const uint32_t indexSize = 0) { s_RendererAPI->DrawIndexedWireframe(vertexArray, indexSize); }
		inline static void Bind2DTexture(uint32_t id) { s_RendererAPI->Bind2DTexture(id); }
		inline static void Bind2DTexture(uint32_t id, uint32_t indx) { s_RendererAPI->Bind2DTexture(id, indx); }

	};

}
