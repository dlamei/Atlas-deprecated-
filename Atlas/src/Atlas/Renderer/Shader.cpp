#include "Shader.h"
#include "atlpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}
}
