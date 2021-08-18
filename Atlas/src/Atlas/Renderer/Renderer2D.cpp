#include "atlpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

namespace Atlas {

	struct Vertex
	{
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		glm::vec4 m_FillColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		Ref<VertexArray> TriVertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		Ref<VertexArray> QuadVertexArray;

		Ref<Shader> FlatShader;
		Ref<Shader> ScreenShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t IndexCount = 0;
		uint32_t VertexCount = 0;

		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;

		uint32_t* IndexBufferBase = nullptr;
		uint32_t* IndexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots>  TextureSlots;
		uint32_t TextureSlotIndex = 1;

		Renderer2D::RenderStats Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ATL_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();
		float verts[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,	1.0f, 1.0f, 1.0f,
			-1.0f,	1.0f, 0.0f, 1.0f
		};

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(verts, 4 * 4 * sizeof(float));
		quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(indices, 6);
		s_Data.QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		s_Data.TriVertexArray = VertexArray::Create();

		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex));
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_Texture" },
			{ ShaderDataType::Float, "aTexIndex" }
			});

		s_Data.IndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);

		s_Data.VertexBufferBase = new Vertex[s_Data.MaxVertices];
		s_Data.IndexBufferBase = new uint32_t[s_Data.MaxIndices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.FlatShader = Atlas::Shader::Create("assets/Shaders/FlatColor.glsl");
		s_Data.FlatShader->Bind();

		s_Data.ScreenShader = Atlas::Shader::Create("assets/Shaders/Screen.glsl");

		int32_t samplers[s_Data.MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}
		s_Data.FlatShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TriVertexArray->AddVertexBuffer(s_Data.VertexBuffer);
		s_Data.TriVertexArray->SetIndexBuffer(s_Data.IndexBuffer);
	}

	void Renderer2D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();

		delete[] s_Data.VertexBufferBase;
		delete s_Data.VertexBufferPtr;

		delete[] s_Data.IndexBufferBase;
		delete s_Data.IndexBufferPtr;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ATL_PROFILE_FUNCTION();
		//TODO
		s_Data.TriVertexArray->BindAll();

		s_Data.FlatShader->Bind();
		s_Data.FlatShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.IndexCount = 0;
		s_Data.VertexCount = 0;

		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		s_Data.IndexBufferPtr = s_Data.IndexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	//TODO: what if nothing is drawn
	void Renderer2D::EndScene()
	{
		ATL_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
		s_Data.VertexBuffer->SetData(s_Data.VertexBufferBase, dataSize);

		dataSize = (uint32_t)((uint8_t*)s_Data.IndexBufferPtr - (uint8_t*)s_Data.IndexBufferBase);
		s_Data.IndexBuffer->SetData(s_Data.IndexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		ATL_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.TriVertexArray, s_Data.IndexCount);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushReset()
	{
		EndScene();

		s_Data.IndexCount = 0;
		s_Data.VertexCount = 0;

		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		s_Data.IndexBufferPtr = s_Data.IndexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::SetFill(const glm::vec3& color)
	{
		s_Data.m_FillColor = { color.r, color.g, color.b, 1.0f };
	}

	void Renderer2D::SetFill(const glm::vec4& color)
	{
		s_Data.m_FillColor = color;
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size)
	{
		DrawRect(position, size, s_Data.m_FillColor);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		DrawRect(position, size, glm::vec4(color.r, color.g, color.b, 1.0f));
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		ATL_PROFILE_FUNCTION();

		if (s_Data.VertexCount + 4 > Renderer2DData::MaxVertices)
		{
			FlushReset();
		}

		if (s_Data.IndexCount + 6 > Renderer2DData::MaxIndices)
		{
			FlushReset();
		}

		const float texIndex = 0.0f;

		s_Data.VertexBufferPtr->Position = position;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = { position.x + size.x, position.y };
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = { position.x + size.x, position.y + size.y };
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = { position.x, position.y + size.y };
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;

		*s_Data.IndexBufferPtr = s_Data.VertexCount + 0;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 1;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 2;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 2;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 3;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 0;			
		s_Data.IndexBufferPtr++;

		s_Data.IndexCount += 6;
		s_Data.VertexCount += 4;

		s_Data.Stats.QuadCount++;

	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		ATL_PROFILE_FUNCTION();

		if (s_Data.VertexCount + 4 > Renderer2DData::MaxVertices)
		{
			FlushReset();
		}

		if (s_Data.IndexCount + 6 > Renderer2DData::MaxIndices)
		{
			FlushReset();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.VertexBufferPtr->Position = position;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.VertexBufferPtr->TexIndex = textureIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = { position.x + size.x, position.y };
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.VertexBufferPtr->TexIndex = textureIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = { position.x + size.x, position.y + size.y };
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.VertexBufferPtr->TexIndex = textureIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = { position.x, position.y + size.y };
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.VertexBufferPtr->TexIndex = textureIndex;
		s_Data.VertexBufferPtr++;

		*s_Data.IndexBufferPtr = s_Data.VertexCount + 0;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 1;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 2;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 2;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 3;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 0;			
		s_Data.IndexBufferPtr++;

		s_Data.IndexCount += 6;
		s_Data.VertexCount += 4;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawTri(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3)
	{
		DrawTri(pos1, pos2, pos3, s_Data.m_FillColor);
	}

	void Renderer2D::DrawTri(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3, const glm::vec3& color)
	{
		DrawTri(pos1, pos2, pos3, { color.r, color.g, color.g, 1.0f });
	}

	void Renderer2D::DrawTri(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3, const glm::vec4& color)
	{
		ATL_PROFILE_FUNCTION();

		if (s_Data.VertexCount + 3 > Renderer2DData::MaxVertices)
		{
			FlushReset();
		}

		if (s_Data.IndexCount + 3 > Renderer2DData::MaxIndices)
		{
			FlushReset();
		}

		const float texIndex = 0.0f;

		s_Data.VertexBufferPtr->Position = pos1;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = pos2;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = pos3;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.VertexBufferPtr->TexIndex = texIndex;
		s_Data.VertexBufferPtr++;
		
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 0;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 1;			
		s_Data.IndexBufferPtr++;
		*s_Data.IndexBufferPtr = s_Data.VertexCount + 2;
		s_Data.IndexBufferPtr++;

		s_Data.IndexCount += 3;
		s_Data.VertexCount += 3;

		s_Data.Stats.TriCount++;
	}

	void Renderer2D::DrawFrameBuffer(uint32_t id)
	{
		glm::mat4 camera = glm::ortho(-1, 1, -1, 1);

		s_Data.ScreenShader->Bind();
		s_Data.QuadVertexArray->BindAll();
		glBindTexture(GL_TEXTURE_2D, id);
		Atlas::RenderCommand::DrawIndexed(s_Data.QuadVertexArray, 6);

		s_Data.FlatShader->Bind();
	} 

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(RenderStats));
	}

	Renderer2D::RenderStats Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}
