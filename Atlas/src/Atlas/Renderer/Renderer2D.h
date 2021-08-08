#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"

namespace Atlas {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	};

}
