#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Atlas {

	class Shader
	{
	private:
		uint32_t m_RendererID = -1;

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	 };

}
