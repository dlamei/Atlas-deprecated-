#include "atlpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Atlas {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		ATL_CORE_ASSERT(false, "RendererAPI is no t supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		ATL_CORE_ASSERT(false, "RendererAPI is no t supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLIndexBuffer(indices, count);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

}
