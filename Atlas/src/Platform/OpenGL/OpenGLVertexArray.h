#pragma once

#include "Atlas/Renderer/VertexArray.h"

namespace Atlas {

	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferOffset = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	};

}
