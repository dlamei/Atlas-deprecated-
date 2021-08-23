#pragma once

#include "Atlas/Core/Core.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Atlas/Core/Core.h"
#include "Atlas/Renderer/VertexArray.h"

namespace Atlas {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal = glm::vec3(0.0f);
		glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	};

	struct Triangle
	{
		uint32_t indices[3];
	};

	class Mesh
	{
	private:
		uint32_t m_VertexCount = 0, m_TriangleCount = 0;
		Triangle* m_Triangles = nullptr;
		Vertex* m_Vertices = nullptr;

		glm::mat4 m_TranslationMatrix = glm::mat4(1.0f), m_RotationMatrix = glm::mat4(1.0f);

		Ref<VertexArray> m_VertexArray;

	public:
		Mesh(const char* path);
		~Mesh();

		static Ref<Mesh> Create(const char* path);

		bool Load(const char* path);
		void Invalidate();
		void CalculateNormals();

		inline uint32_t GetTriangleCount() const { return m_TriangleCount; }
		inline uint32_t GetIndexCount() const { return m_TriangleCount * 3; }
		inline uint32_t GetVertexCount() const { return m_VertexCount; }
		inline Triangle* GetTriangles() const { return m_Triangles; }
		inline Vertex* GetVertex(const uint32_t index) const { ATL_CORE_ASSERT(!(index >= m_VertexCount), "Index: out of bounds!"); return &m_Vertices[index]; }
		inline Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

		inline glm::mat4& GetTranslationMatrix() { return m_TranslationMatrix; }
		inline glm::mat4& GetRotationMatrix() { return m_RotationMatrix; }

		inline void SetTranslation(const glm::vec3 position) { m_TranslationMatrix = glm::translate(glm::mat4(1.0f), position); }
		inline void SetRotation(const glm::vec3 axis, const float angle) { m_RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis); }
	};

}
