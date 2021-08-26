#include "atlpch.h"

#include "Mesh.h"
#include "Renderer.h"

namespace std {
	template <>
	struct hash<glm::vec3>
	{
		std::size_t operator()(const glm::vec3& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<float>()(k.x)
				^ (hash<float>()(k.y) << 1)) >> 1)
				^ (hash<float>()(k.z) << 1);

		}
	};
}

namespace Atlas {
	Ref<Mesh> Mesh::Create(const char* path)
	{
		return std::make_shared<Mesh>(path);
	}

	Mesh::Mesh(const char* path)
	{
		Load(path);
	}

	Mesh::~Mesh()
	{
		delete[] m_VertexTriangles;
		delete[] m_Indices;
		delete[] m_Textures;
	}

	bool Mesh::Load(const char* path)
	{
		{
			std::ifstream file(path);

			if (!file.is_open())
			{
				ATL_CORE_WARN("Could not open file: {0}", path);
				return false;
			}

			while (!file.eof())
			{
				char line[128];
				file.getline(line, 128);
				if (line[0] == 'v')
				{
					if (line[1] == 't') m_TexterCoordCount++;
					else m_VertexCount++;
				}
				else if (line[0] == 'f') m_TriangleCount++;
			}
			file.close();
		}

		m_VertexTriangles = new VertexTriangle[m_TriangleCount];
		m_Indices = new uint32_t[m_TriangleCount * 3];

		VertexTriangle* vertexTrianglePtr = m_VertexTriangles;
		uint32_t* indexPtr = m_Indices;

		std::ifstream file(path);

		file.seekg(0);
		file.clear();

		std::vector<glm::vec3> vertexPos;
		std::vector<glm::vec2> vertexTexCoords;
		std::vector<std::tuple<int, int, int>> trianglePosIndices;
		std::vector<std::tuple<int, int, int>> triangleTexCoordIndices;

		vertexPos.reserve(m_VertexCount);
		vertexTexCoords.reserve(m_TexterCoordCount);
		trianglePosIndices.reserve(m_TriangleCount);
		triangleTexCoordIndices.reserve(m_TriangleCount);

		while (!file.eof())
		{
			char line[128];

			file.getline(line, 128);

			std::stringstream strStream;
			strStream << line;

			char c;

			if (line[0] == 'v')
			{
				float x = 0, y = 0, z = 0;

				if (line[1] == 't')
				{
					m_HasTexCoords = true;
					strStream >> c >> c >> x >> y;
					vertexTexCoords.push_back({ x, y });
				}
				else
				{
					strStream >> c >> x >> y >> z;
					vertexPos.push_back({ x, y, z });
				}
			}
			else if (line[0] == 'f')
			{
				char indx[3][12]{};

				strStream >> c >> indx[0] >> indx[1] >> indx[2];


				int texCoords[3][2]{};
				for (int i = 0; i < 3; i++)
				{
					std::stringstream s;
					s << indx[i];
					std::string token;

					int counter = 0;
					while (std::getline(s, token, '/'))
					{
						if (counter < 2) texCoords[i][counter] = std::stoi(token) - 1;
						counter++;
					}
				}

				trianglePosIndices.push_back({ texCoords[0][0], texCoords[1][0], texCoords[2][0] });

				if (m_HasTexCoords)
				{
					triangleTexCoordIndices.push_back({ texCoords[0][1], texCoords[1][1], texCoords[2][1] });
				}
			}
		}

		file.close();

		for (uint32_t i = 0; i < m_TriangleCount; i++)
		{
			auto& posIndx = trianglePosIndices.at(i);

			auto& [ix, iy, iz] = posIndx;
			auto& p0 = vertexPos.at(ix);
			auto& p1 = vertexPos.at(iy);
			auto& p2 = vertexPos.at(iz);

			vertexTrianglePtr->Vertices[0].Position = p0;
			vertexTrianglePtr->Vertices[1].Position = p1;
			vertexTrianglePtr->Vertices[2].Position = p2;

			if (m_HasTexCoords)
			{
				auto& texCoordIndx = triangleTexCoordIndices.at(i);
				auto& [ivx, ivy, ivz] = texCoordIndx;
				auto& t0 = vertexTexCoords.at(ivx);
				auto& t1 = vertexTexCoords.at(ivy);
				auto& t2 = vertexTexCoords.at(ivz);

				vertexTrianglePtr->Vertices[0].TextureCoord = t0;
				vertexTrianglePtr->Vertices[1].TextureCoord = t1;
				vertexTrianglePtr->Vertices[2].TextureCoord = t2;
			}

			m_Indices[3 * i + 0] = 3 * i + 0;
			m_Indices[3 * i + 1] = 3 * i + 1;
			m_Indices[3 * i + 2] = 3 * i + 2;

			vertexTrianglePtr++;
		}

		CalculateNormals();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(m_TriangleCount * 3 * sizeof(Vertex));
		vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TextureCoord" },
			});
		vertexBuffer->SetData(m_VertexTriangles, m_TriangleCount * 3 * sizeof(Vertex));

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_TriangleCount * 3 * sizeof(uint32_t));
		indexBuffer->SetData(m_Indices, sizeof(uint32_t), m_TriangleCount * 3);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		return true;
	}

	void Mesh::Invalidate()
	{
		m_VertexArray->GetVertexBuffer()->SetData(m_VertexTriangles, m_TriangleCount * 3 * sizeof(Vertex));
		m_VertexArray->GetIndexBuffer()->SetData(m_Indices, sizeof(uint32_t), m_TriangleCount * 3);
	}



	void Mesh::CalculateNormals()
	{


		std::unordered_map<glm::vec3, int> repetitionCount;
		std::unordered_map<glm::vec3, glm::vec3> normalMap;

		for (uint32_t i = 0; i < m_TriangleCount; i++)
		{
			VertexTriangle& tri = m_VertexTriangles[i];

			Vertex& v0 = tri.Vertices[0];
			Vertex& v1 = tri.Vertices[1];
			Vertex& v2 = tri.Vertices[2];

			glm::vec3 vec1 = v1.Position - v0.Position;
			glm::vec3 vec2 = v2.Position - v0.Position;

			glm::vec3 normal = glm::normalize(glm::cross(vec1, vec2));

			if (!m_SmoothShading)
			{
				v0.Normal = normal;
				v1.Normal = normal;
				v2.Normal = normal;
			}
			else 
			{
				normalMap[v0.Position] += normal;
				normalMap[v1.Position] += normal;
				normalMap[v2.Position] += normal;

				repetitionCount[v0.Position] += 1;
				repetitionCount[v1.Position] += 1;
				repetitionCount[v2.Position] += 1;
			}
		}

		if (m_SmoothShading)
		{
			for (uint32_t i = 0; i < m_TriangleCount; i++)
			{
				VertexTriangle& tri = m_VertexTriangles[i];

				Vertex& v0 = tri.Vertices[0];
				Vertex& v1 = tri.Vertices[1];
				Vertex& v2 = tri.Vertices[2];

				v0.Normal = normalMap[v0.Position] / (float)repetitionCount[v0.Position];
				v1.Normal = normalMap[v1.Position] / (float)repetitionCount[v1.Position];
				v2.Normal = normalMap[v2.Position] / (float)repetitionCount[v2.Position];
			}
		}
	}

	void Mesh::SetShading(bool smooth)
	{
		if (smooth != m_SmoothShading)
		{
			m_SmoothShading = smooth;
			CalculateNormals();
			Invalidate();
		}
	}
}

