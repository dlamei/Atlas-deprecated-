#include "atlpch.h"
#include "FrameBuffer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer.h"

namespace Atlas
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecs& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLFrameBuffer>(specs);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}
}