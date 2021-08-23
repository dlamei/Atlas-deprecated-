#pragma once

#include "Atlas/Core/Core.h"

#include <string>
#include <glm/glm.hpp>

#include "Atlas/Core/Core.h"
#include "Atlas/Renderer/VertexArray.h"

namespace Atlas {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal = glm::vec3(0.0f);
		glm::vec4 Color = glm::vec4(1.0f);
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
		inline Vertex* GetVertex(uint32_t index) const { ATL_CORE_ASSERT(!(index >= m_VertexCount), "Index: out of bounds!"); return &m_Vertices[index]; }
		inline Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }
	};

}
