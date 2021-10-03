#include "atlpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Atlas {
	GLenum AtlToGlEnum(Utils::Operation operation)
	{
		switch (operation)
		{
		case Utils::Operation::ALLWAYS: return GL_ALWAYS;
		case Utils::Operation::NOTEQUAL: return GL_NOTEQUAL;
		case Utils::Operation::LESS: return GL_LESS;
		case Utils::Operation::LEQUAL: return GL_LEQUAL;
		case Utils::Operation::STENCIL: return GL_STENCIL_TEST;
		case Utils::Operation::DEPTH: return GL_DEPTH_TEST;
		case Utils::Operation::CULL_FACES: return GL_DEPTH_TEST;
		case Utils::Operation::KEEP: return GL_KEEP;
		case Utils::Operation::REPLACE: return GL_REPLACE;
		case Utils::Operation::EQUAL: return GL_EQUAL;
		}

		ATL_CORE_ASSERT(false, "Unknown operation");
		return GL_NONE;
	}


	void OpenGLRendererAPI::Init()
	{
		ATL_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

		//glEnable(GL_CULL_FACE);

		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_EQUAL, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Enable(Utils::Operation operation)
	{
		glEnable(AtlToGlEnum(operation));
	}

	void OpenGLRendererAPI::Disable(Utils::Operation operation)
	{
		glDisable(AtlToGlEnum(operation));
	}

	void OpenGLRendererAPI::SetStencilOp(Utils::Operation op1, Utils::Operation op2, Utils::Operation op3)
	{
		glStencilOp(AtlToGlEnum(op1), AtlToGlEnum(op2), AtlToGlEnum(op3));
	}

	void OpenGLRendererAPI::SetStencilFunc(Utils::Operation operation, int ref, uint32_t mask)
	{
		glStencilFunc(AtlToGlEnum(operation), ref, mask);
	}

	void OpenGLRendererAPI::SetStencilMask(uint32_t mask)
	{
		glStencilMask(mask);
	}

	void OpenGLRendererAPI::SetDepthFunc(Utils::Operation operation)
	{
		glDepthFunc(AtlToGlEnum(operation));
	}

	void OpenGLRendererAPI::DrawIndexedWireframe(const Ref<VertexArray>& vertexArray, const uint32_t indexCount)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);

		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t indexCount)
	{
		uint32_t count = indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawPoints(const Ref<VertexArray>& vertexArray, const uint32_t indexCount)
	{
		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, nullptr);
	}


	void OpenGLRendererAPI::Bind2DTexture(uint32_t id)
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
}