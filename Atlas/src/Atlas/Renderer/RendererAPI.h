#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Atlas {

	namespace Utils
	{
		enum class Operation
		{
			ALLWAYS = 0,
			NOTEQUAL,
			LESS,
			LEQUAL,
			EQUAL,

			STENCIL,
			DEPTH,

			CULL_FACES,

			KEEP,
			REPLACE
		};
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	private:
		static API s_API;

	public:

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void Enable(Utils::Operation operation) = 0;
		virtual void Disable(Utils::Operation operation) = 0;

		virtual void SetStencilOp(Utils::Operation op1, Utils::Operation op2, Utils::Operation op3) = 0;
		virtual void SetStencilFunc(Utils::Operation operation, int ref, uint32_t mask) = 0;
		virtual void SetStencilMask(uint32_t mask) = 0;

		virtual void SetDepthFunc(Utils::Operation operation) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) = 0;
		virtual void DrawIndexedWireframe(const Ref<VertexArray>& verteyArray, const uint32_t indexCount) = 0;
		virtual void DrawPoints(const Ref<VertexArray>& vertexArray, const uint32_t indexCount) = 0;

		virtual void Bind2DTexture(uint32_t id) = 0;

		inline static API GetAPI() { return s_API; }
		
	};

}
