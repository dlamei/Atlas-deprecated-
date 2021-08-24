#include "atlpch.h"

#include "Mesh.h"
#include "Renderer.h"

namespace Atlas {
	Ref<Mesh> Mesh::Create(const char* path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	ATL_CORE_ASSERT(false, "RendererAPI is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<Mesh>(path);
		}

		ATL_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	Mesh::Mesh(const char* path)
	{
		Load(path);
	}

	Mesh::~Mesh()
	{
		delete[] m_Triangles;
		delete[] m_Vertices;
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
				if (line[0] == 'v') m_VertexCount++;
				else if (line[0] == 'f') m_TriangleCount++;
			}
			file.close();
		}

		m_Vertices = new Vertex[m_VertexCount];
		m_Triangles = new Triangle[m_TriangleCount];

		Vertex* vertexPtr = m_Vertices;
		Triangle* trianglePtr = m_Triangles;

		std::ifstream file(path);

		file.seekg(0);
		file.clear();

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
				strStream >> c >> x >> y >> z;
				vertexPtr->Position = { x, y, z };
				vertexPtr++;
			}
			else if (line[0] == 'f')
			{
				int indx[3]{};
				strStream >> c >> indx[0] >> indx[1] >> indx[2];
				trianglePtr->indices[0] = indx[0] - 1;
				trianglePtr->indices[1] = indx[1] - 1;
				trianglePtr->indices[2] = indx[2] - 1;
				trianglePtr++;
			}
		}

		file.close();

		CalculateNormals();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(m_VertexCount * sizeof(Vertex));
		vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
			});
		vertexBuffer->SetData(m_Vertices, m_VertexCount * sizeof(Vertex));

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(m_TriangleCount * sizeof(Triangle));
		indexBuffer->SetData(m_Triangles, sizeof(uint32_t), m_TriangleCount * 3);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		return true;
	}

	void Mesh::Invalidate()
	{
		m_VertexArray->GetVertexBuffer()->SetData(m_Vertices, m_VertexCount * sizeof(Vertex));
		m_VertexArray->GetIndexBuffer()->SetData(m_Triangles, sizeof(uint32_t), m_TriangleCount * 3);
	}

	void Mesh::CalculateNormals()
	{
		for (uint32_t i = 0; i < m_TriangleCount; i++)
		{
			Triangle tri = m_Triangles[i];
			
			Vertex* v0 = GetVertex(tri.indices[0]);
			Vertex* v1 = GetVertex(tri.indices[1]);
			Vertex* v2 = GetVertex(tri.indices[2]);

			glm::vec3 vec1 = v1->Position - v0->Position;
			glm::vec3 vec2 = v2->Position - v0->Position;

			glm::vec3 normal = glm::normalize(glm::cross(vec1, vec2));

			v0->Normal = normal;
			v1->Normal = normal;
			v2->Normal = normal;
		}

	}

}
