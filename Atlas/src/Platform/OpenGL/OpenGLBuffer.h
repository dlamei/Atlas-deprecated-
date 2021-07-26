#pragma once

#include "Atlas/Renderer/Buffer.h"

namespace Atlas {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_RendererID;

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};




	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
		
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	};

}
