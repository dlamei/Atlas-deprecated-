#pragma once

#include "Atlas/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Atlas {

	class OpenGLShader : public Shader
	{
	private:
		uint32_t m_RendererID = -1;

		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrc);
		std::unordered_map<GLenum, std::string>  PreProcess(const std::string& source);
		std::string ReadFile(const std::string& filepath);

	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetUnifromInt(const std::string& name, const int value);

		void SetUnifromFloat(const std::string& name, const float value);
		void SetUnifromFloat2(const std::string& name, const glm::vec2& values);
		void SetUnifromFloat3(const std::string& name, const glm::vec3& values);
		void SetUnifromFloat4(const std::string& name, const glm::vec4& values);

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	};

}
