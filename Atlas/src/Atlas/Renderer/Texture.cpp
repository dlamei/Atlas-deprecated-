#include "atlpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Atlas {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, bool flipped)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path, flipped);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
	//Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t id);
	//{
	//	switch (Renderer::GetAPI())
	//	{
	//		case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
	//		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height, id);
	//	}

	//	ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
	//		return nullptr;
	//}

