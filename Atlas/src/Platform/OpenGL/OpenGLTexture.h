#pragma once

#include "Atlas/Renderer/Texture.h"

#include <glad/glad.h>

namespace Atlas {

	class OpenGLTexture2D : public Texture2D
	{
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_DataFormat, m_InternalFormat;

	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual inline uint32_t GetWidth() const override { return m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Height; }
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind(uint32_t slot) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	};

}
