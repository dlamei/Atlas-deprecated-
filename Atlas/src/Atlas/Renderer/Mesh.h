#pragma once

#include "Atlas/Core/Core.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "Texture.h"

namespace Atlas {

	namespace Utils {

		enum class TextureType
		{
			DIFFUSE = 0,
			SPECULAR,
			NORMAL,
			NONE
			
		};
	}

	struct Vertex
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f);
		glm::vec2 TextureCoord = glm::vec2(0.0f);
	};

	struct VertexTriangle
	{
		Vertex Vertices[3];
	};

	struct Triangle
	{
		uint32_t indices[3];
	};

	class Mesh
	{
	private:
		bool m_HasTexCoords = false;
		bool m_SmoothShading = true;

		std::string m_Name;

		uint32_t m_VertexCount = 0, m_TriangleCount = 0, m_TextureCoordCount = 0;
		VertexTriangle* m_VertexTriangles = nullptr;
		uint32_t* m_Indices = nullptr;


		glm::mat4 m_Transform = glm::mat4(1.0f);

		Ref<VertexArray> m_VertexArray;
		std::array<Ref<Texture2D>, (int)Utils::TextureType::NONE> m_Textures;


	public:
		Mesh();
		Mesh(const std::string& path);
		~Mesh();

		void InitTextures();

		void Load(const std::string& path);
		void Invalidate();
		void CalculateNormals();
		void SetShading(bool smooth);

		inline uint32_t GetTriangleCount() const { return m_TriangleCount; }
		inline uint32_t GetIndexCount() const { return m_TriangleCount * 3; }
		inline uint32_t GetVertexCount() const { return m_TriangleCount * 3; }
		inline VertexTriangle* GetTriangles() const { return m_VertexTriangles; }
		inline Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

		glm::mat4& GetTransformMatrix() { return m_Transform; }

		void SetTransfrom(const glm::mat4 transform) { m_Transform = transform; }

		inline std::string& GetName() { return m_Name; }

		inline void AddTexture(const Ref<Texture2D> texture, const Utils::TextureType type) { if (type != Utils::TextureType::NONE) m_Textures[(int)type] = texture; }
		inline Ref<Texture2D> GetTexture(const Utils::TextureType type) { return m_Textures[(int)type]; }
		inline void BindTexture(Utils::TextureType type) { m_Textures[(int)type]->Bind((int)type); }
	};

}
