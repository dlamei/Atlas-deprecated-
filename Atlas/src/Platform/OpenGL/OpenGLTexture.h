#pragma once

#include "Atlas/Renderer/Texture.h"

namespace Atlas {

	class OpenGLTexture2D : public Texture2D
	{
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;

	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual inline uint32_t GetWidth() const override { return m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
	};

}
