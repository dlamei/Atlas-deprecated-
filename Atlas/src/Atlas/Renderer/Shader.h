#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Atlas {

	namespace Utils {

		enum class ShaderType
		{
			FRAGMENT = 0,
			VERTEX,
			GEOMETRY
		};
	}

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;

		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;

		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& src);

		virtual bool operator==(const Shader& other) const = 0;
		virtual bool operator!=(const Shader& other) const = 0;

	 };

	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name);
	};

	class ShaderManager
	{
	private:
		static const Ref<Shader>& m_ActiveShader;

	public:
		static void BindShader(const Ref<Shader>& shader)
		{
			if (*m_ActiveShader != *shader)
			{
				*m_ActiveShader = *shader;
				m_ActiveShader->Bind();
			}
		}

		static const Ref<Shader>& GetActiveShader()
		{
			return m_ActiveShader;
		}
	};


}
