#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Renderer/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace Atlas {

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
		MeshComponent() = default;
		MeshComponent(const std::string& path)
			: Mesh(CreateRef<Atlas::Mesh>(path)) {}

		operator Ref<Atlas::Mesh>& () { return Mesh; }
		operator Atlas::Mesh& () { return *Mesh; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

}
