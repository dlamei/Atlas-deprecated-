#pragma once

#include <glm/glm.hpp>

namespace Atlas {
	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
		//bool decompose(const glm::mat4& ModelMatrix, glm::vec3& Scale, glm::quat& Orientation, glm::vec3& Translation);
		bool decompose(const glm::mat4& ModelMatrix, glm::vec3& Scale, glm::quat& Orientation, glm::vec3& Translation, glm::vec3& Skew, glm::vec4& Perspective);

	}
}
