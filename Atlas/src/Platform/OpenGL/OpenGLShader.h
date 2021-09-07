#pragma once

#include "Atlas/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Atlas {

	class OpenGLShader : public Shader
	{
	private:
		std::string m_Name;

		uint32_t m_RendererID = -1;

		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrc);

		std::unordered_map<GLenum, std::string>  PreProcess(const std::string& source);
		std::unordered_map<std::string, int> m_UnifromLocationChache;
		std::string ReadFile(const std::string& filepath);

		int GetUniformLocation(const std::string& name);

	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual void SetFloat(const std::string& name, const float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) override;

		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void SetUnifromInt(const std::string& name, const int value);
		void SetUnifromIntArray(const std::string& name, const int* values, uint32_t count);

		void SetUnifromFloat(const std::string& name, const float value);
		void SetUnifromFloat2(const std::string& name, const glm::vec2& values);
		void SetUnifromFloat3(const std::string& name, const glm::vec3& values);
		void SetUnifromFloat4(const std::string& name, const glm::vec4& values);

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

		virtual bool operator==(const Shader& other) const override
		{
			return m_RendererID == ((OpenGLShader&)other).m_RendererID;
		}

		virtual bool operator!=(const Shader& other) const override
		{
			return m_RendererID != ((OpenGLShader&)other).m_RendererID;
		}
	};

}
