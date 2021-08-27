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
		ATL_PROFILE_FUNCTION();

		/*
		{
			ATL_PROFILE_SCOPE("first vertex count");

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
					if (line[1] == 't') m_TextureCoordCount++;
					else m_VertexCount++;
				}
				else if (line[0] == 'f') m_TriangleCount++;
			}
			file.close();
		}
		*/


		std::ifstream file(path);

		file.seekg(0);
		file.clear();

		std::vector<glm::vec3> vertexPos;
		std::vector<glm::vec3> vertexNormals;
		std::vector<glm::vec2> vertexTexCoords;
		std::vector<std::tuple<int, int, int>> trianglePosIndices;
		std::vector<std::tuple<int, int, int>> triangleTexCoordIndices;
		std::vector<std::tuple<int, int, int>> triangleNormalIndices;

		bool hasVertexNormals = false;

		//vertexPos.reserve(m_VertexCount);
		//vertexTexCoords.reserve(m_TextureCoordCount);
		//trianglePosIndices.reserve(m_TriangleCount);
		//triangleTexCoordIndices.reserve(m_TriangleCount);

		std::string line;
		std::string numbers;

		while (std::getline(file, line))
		{

			if (line[0] == 'v')
			{

				if (line[1] == 't')
				{
					m_TextureCoordCount++;
					m_HasTexCoords = true;
					numbers = line.substr(3, line.size() - 1);

					char num[2][16]{};

					short indx = 0;
					short numCount = 0;
					for (char c : numbers)
					{
						if (c == ' ')
						{
							indx = 0;
							numCount += 1;
							ATL_CORE_ASSERT(numCount <= 1, "invalid format for Texture Coords!");
						}

						if (indx >= 16) break;
						num[numCount][indx] = c;

						indx++;
					}

					vertexTexCoords.push_back({ (float)std::atof(num[0]), (float)std::atof(num[1]) });

				}
				else if (line[1] == 'n')
				{
					numbers = line.substr(3, line.size() - 1);
					hasVertexNormals = true;
					char num[3][16]{};

					short indx = 0;
					short numCount = 0;
					for (char c : numbers)
					{
						if (c == ' ')
						{
							indx = 0;
							numCount += 1;
							if (numCount > 2) break;
						}

						if (indx >= 16) break;
						num[numCount][indx] = c;

						indx++;
					}

					vertexNormals.push_back({ (float)std::atof(num[0]), (float)std::atof(num[1]), std::atof(num[2]) });
				}
				else
				{
					m_VertexCount++;
					numbers = line.substr(2, line.size() - 1);
					char num[3][16]{};

					short indx = 0;
					short numCount = 0;
					for (char c : numbers)
					{
						if (c == ' ')
						{
							indx = 0;
							numCount += 1;
							if (numCount > 2) break;
							//ATL_CORE_ASSERT(numCount <= 2, "Atlas 3D only supports triangles!");
						}

						if (indx >= 16) break;
						num[numCount][indx] = c;

						indx++;
					}

					vertexPos.push_back({ (float)std::atof(num[0]), (float)std::atof(num[1]), std::atof(num[2]) });

				}
			}

			if (line[0] == 'f')
			{
				m_TriangleCount++;
				numbers = line.substr(2, line.size() - 1);

				char vertexIndecis[3][16]{};
				char texCoordIndecis[3][16]{};
				char vertexNormalIndecis[3][16]{};

				short vertexIndx = 0;
				short texCoordIndx = 0;
				short vertexNormalIndx = 0;
				short indxCount = 0;

				short indxType = 0;

				bool skip = false;

				for (char c : numbers)
				{
					if (!skip)
					{
						if (c == '/')
						{
							indxType++;
							if (indxType > 2)
							{
								skip = true;
							}

							continue;
						}

						if (indxType == 0)
						{
							vertexIndecis[indxCount][vertexIndx] = c;
							vertexIndx++;
						}
						else if (indxType == 1)
						{
							texCoordIndecis[indxCount][texCoordIndx] = c;
							texCoordIndx++;
						}
						else if (indxType == 2)
						{
							vertexNormalIndecis[indxCount][vertexNormalIndx] = c;
							vertexNormalIndx++;
						}
					}

					if (skip && c == ' ') skip = false;
					
					if (c == ' ')
					{
						indxCount++;
						vertexIndx = 0;
						vertexNormalIndx = 0;
						texCoordIndx = 0;
						indxType = 0;
					}
				}

				trianglePosIndices.push_back({ (int)std::atof(vertexIndecis[0]) - 1, (int)std::atof(vertexIndecis[1]) - 1, (int)std::atof(vertexIndecis[2]) - 1 });

				if (m_HasTexCoords) triangleTexCoordIndices.push_back({ (int)std::atof(texCoordIndecis[0]) - 1, (int)std::atof(texCoordIndecis[1]) - 1, (int)std::atof(texCoordIndecis[2]) - 1 });
				if (hasVertexNormals) triangleNormalIndices.push_back({ (int)std::atof(vertexNormalIndecis[0]) - 1, (int)std::atof(vertexNormalIndecis[1]) - 1, (int)std::atof(vertexNormalIndecis[2]) - 1 });
			}
		}

		m_VertexTriangles = new VertexTriangle[m_TriangleCount];
		m_Indices = new uint32_t[m_TriangleCount * 3];

		VertexTriangle* vertexTrianglePtr = m_VertexTriangles;
		uint32_t* indexPtr = m_Indices;

		file.close();

		{
			ATL_PROFILE_SCOPE("put vertex into buffer");

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

				if (hasVertexNormals)
				{
					auto& normalIndx = triangleNormalIndices.at(i);
					auto& [inx, iny, inz] = normalIndx;
					auto& n0 = vertexNormals.at(inx);
					auto& n1 = vertexNormals.at(iny);
					auto& n2 = vertexNormals.at(inz);

					vertexTrianglePtr->Vertices[0].Normal = n0;
					vertexTrianglePtr->Vertices[1].Normal = n1;
					vertexTrianglePtr->Vertices[2].Normal = n2;
				}

				m_Indices[3 * i + 0] = 3 * i + 0;
				m_Indices[3 * i + 1] = 3 * i + 1;
				m_Indices[3 * i + 2] = 3 * i + 2;

				vertexTrianglePtr++;
			}
		}

		if (!hasVertexNormals) CalculateNormals();

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
		ATL_PROFILE_FUNCTION();

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

