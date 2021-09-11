#pragma once

#include <string>

namespace Atlas {

	struct TagComponent
	{
		const char* Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const char* tag)
			: Tag(tag) {}

		operator const char*& () { return Tag; }

	};

}
