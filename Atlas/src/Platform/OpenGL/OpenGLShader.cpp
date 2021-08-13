#include "atlpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <filesystem>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		ATL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static std::string StringFromShaderType(const GLenum type)
	{
		if (type == GL_VERTEX_SHADER) return "vertex";
		if (type == GL_FRAGMENT_SHADER) return "fragment";
		return "unknown";
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		ATL_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}


	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		ATL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		ATL_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		ATL_PROFILE_FUNCTION();

		std::string result;
		std::ifstream inStream(filepath, std::ios::in | std::ios::binary);
		if (inStream)
		{
			inStream.seekg(0, std::ios::end);
			result.resize(inStream.tellg());
			inStream.seekg(0, std::ios::beg);
			inStream.read(&result[0], result.size());
			inStream.close();
		}
		else
		{
			ATL_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		ATL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			ATL_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); 
			ATL_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); 
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		ATL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.resize(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			auto& [type, source] = kv;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				ATL_CORE_ERROR("{0}", infoLog.data());
				ATL_CORE_ASSERT(false, StringFromShaderType(type) + " shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);

			for (auto shaderID : glShaderIDs)
			{
				glDetachShader(program, shaderID);
				glDeleteShader(shaderID);
			}

			ATL_CORE_ERROR("{0}", infoLog.data());
			ATL_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto shaderID : glShaderIDs)
		{
			glDetachShader(program, shaderID);
		}

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		ATL_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		ATL_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value)
	{
		SetUnifromInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		SetUnifromIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value)
	{
		SetUnifromFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		SetUnifromFloat2(name, values);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		SetUnifromFloat3(name, values);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		SetUnifromFloat4(name, values);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		SetUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		SetUniformMat4(name, matrix);
	}

	void OpenGLShader::SetUnifromInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUnifromIntArray(const std::string& name, const int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetUnifromFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetUnifromFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::SetUnifromFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::SetUnifromFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}