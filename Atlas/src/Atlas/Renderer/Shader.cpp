#include "atlpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(filepath);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& src)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(name, src);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		Add(shader->GetName(), shader);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		ATL_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ATL_CORE_ASSERT(Exists(name), "shader not found");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}
