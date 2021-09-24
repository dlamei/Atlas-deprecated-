#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Renderer/Mesh.h"
#include "Atlas/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace Atlas {

	namespace Utils
	{
		enum class Transform
		{
			TRANSLATE = 0,
			ROTATE,
			SCALE
		};
	}

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string tag)
			: Tag(tag) {}

		operator std::string&() { return Tag; }
		operator const char* () { return Tag.c_str(); }
	};

	struct MeshComponent
	{
		Ref<Atlas::Mesh> Mesh;
		bool Hide = false;
		float Shininess = 8.0f;

		MeshComponent()
			: Mesh(CreateRef<Atlas::Mesh>()) {}
		MeshComponent(const std::string& path)
			: Mesh(CreateRef<Atlas::Mesh>(path)) {}

		operator Ref<Atlas::Mesh>& () { return Mesh; }
		operator Atlas::Mesh& () { return *Mesh; }
	};

	struct TransformComponent
	{
		Utils::Transform TransformOperation = Utils::Transform::TRANSLATE;
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct PointLightComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Ambient = glm::vec3(0.2f);
		glm::vec3 Diffuse = glm::vec3(0.5f);
		glm::vec3 Specular = glm::vec3(1.0f);

		float Constant = 0.0f;
		float Linear = 0.0f;
		float Quadratic = 0.3f;

		PointLightComponent() = default;

		void SetUniform(Ref<Shader> shader, int index)
		{
			std::string uniformName = "pointLights[" + std::to_string(index) + "]";
			shader->SetFloat3(uniformName + ".Position", Position);
			shader->SetFloat3(uniformName + ".Ambient", Ambient);
			shader->SetFloat3(uniformName + ".Diffuse", Diffuse);
			shader->SetFloat3(uniformName + ".Specular", Specular);
			shader->SetFloat(uniformName + ".Constant", Constant);
			shader->SetFloat(uniformName + ".Linear", Linear);
			shader->SetFloat(uniformName + ".Quadratic", Quadratic);
		}
	};

	struct DirLightComponent
	{
		glm::vec3 Direction = { 1.0, 0.0, 0.0 };
		glm::vec3 Ambient = glm::vec3(0.2f);
		glm::vec3 Diffuse = glm::vec3(0.5f);
		glm::vec3 Specular = glm::vec3(1.0f);

		DirLightComponent() = default;

		void SetUniform(Ref<Shader> shader, int index)
		{
			std::string uniformName = "dirLights[" + std::to_string(index) + "]";
			shader->SetFloat3(uniformName + ".Direction", Direction);
			shader->SetFloat3(uniformName + ".Ambient", Ambient);
			shader->SetFloat3(uniformName + ".Diffuse", Diffuse);
			shader->SetFloat3(uniformName + ".Specular", Specular);
		}
	};

}
