#pragma once

#include "Atlas/Renderer/FrameBuffer.h"

namespace Atlas {


	class OpenGLFrameBuffer : public FrameBuffer
	{
	private:
		FrameBufferSpecs m_Specification;

		uint32_t m_RendererID;
		uint32_t m_DepthAttachment = 0;

		std::vector<FrameBufferTexSpecs> m_ColorAttachmentSpecs;
		FrameBufferTexSpecs m_DepthAttachmentSpec = FBTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;

	public:
		OpenGLFrameBuffer(const FrameBufferSpecs& specs);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();


		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t indx, int value) override;


		inline virtual const FrameBufferSpecs& GetSpecs() const override { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { ATL_CORE_ASSERT(index < m_ColorAttachments.size(), "Index out of bounds"); return m_ColorAttachments[index]; }
	};
}

