#include "atlpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Atlas {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> FlatShader;
		Ref<Texture> WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ATL_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();


		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "u_Color" },
			{ ShaderDataType::Float2, "v_Texture" }
			});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;

		#pragma warning(disable:6386)

		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.FlatShader = Atlas::Shader::Create("assets/Shaders/Texture.glsl");
		s_Data.FlatShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		ATL_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ATL_PROFILE_FUNCTION();

		s_Data.FlatShader->Bind();
		s_Data.FlatShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		ATL_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t) ((uint8_t*) s_Data.QuadVertexBufferPtr - (uint8_t*) s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		ATL_PROFILE_FUNCTION();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{		
		DrawRect({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{		
		ATL_PROFILE_FUNCTION();

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		//s_Data.QuadVertexArray->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		//s_Data.WhiteTexture->Bind();

		////s_Data.FlatShader->SetFloat4("u_Color", color);
		//s_Data.FlatShader->SetMat4("u_Transform", transform);
		//s_Data.FlatShader->SetFloat("u_TilingFactor", 1.0f);

		//RenderCommand::DrawIndexed(s_Data.QuadVertexArray);

	}
	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawRect({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		ATL_PROFILE_FUNCTION();

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.FlatShader->SetMat4("u_Transform", transform);
		s_Data.FlatShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data.FlatShader->SetFloat("u_TilingFactor", 1.0f);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
}
