#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"
#include "Shader.h"

namespace Atlas {

	class Renderer2D
	{
	private:
		static void FlushReset();

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void SetFlatShader();
		static void SetShader(const Ref<Shader>& shader);

		static void SetFill(const glm::vec3& color);
		static void SetFill(const glm::vec4& color);

		static void DrawRect(const glm::vec2& position, const glm::vec2& size);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawTri(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3);
		static void DrawTri(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3, const glm::vec3& color);
		static void DrawTri(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3, const glm::vec4& color);

		static void DrawFrameBuffer(uint32_t id, uint32_t width, uint32_t height, float gamma);
		static void DrawDebugTexture(uint32_t textureID, uint32_t width, uint32_t height, Ref<Shader> shader);

		struct RenderStats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t TriCount = 0;
	
			uint32_t GetTotalVertexCount() { return QuadCount * 4 + TriCount * 3; }
			uint32_t  GetTotalIndexCount() { return QuadCount * 6 + TriCount * 3; }
		}; 

		static void ResetStats();
		static RenderStats GetStats();
	};

}
