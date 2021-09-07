#pragma once

#include "Atlas/Core/Core.h"

namespace Atlas
{
	enum class FBTextureFormat
	{
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8,
	};

	struct FrameBufferTexSpecs
	{
		FrameBufferTexSpecs() = default;
		FrameBufferTexSpecs(FBTextureFormat format)
			: TextureFormat(format) {}

		FBTextureFormat TextureFormat = FBTextureFormat::None;
	};

	struct FrameBufferAttachmentSpecs
	{
		FrameBufferAttachmentSpecs() = default;
		FrameBufferAttachmentSpecs(std::initializer_list<FrameBufferTexSpecs> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTexSpecs> Attachments;
	};

	struct FrameBufferSpecs
	{
		uint32_t Width = 0, Height = 0;
		FrameBufferAttachmentSpecs Attachments;

		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	private:

	public:
		virtual const FrameBufferSpecs& GetSpecs() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecs& specs);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t indx, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

	};

}
