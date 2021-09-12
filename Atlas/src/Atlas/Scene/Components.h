#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Renderer/Mesh.h"

#include <string>

namespace Atlas {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string tag)
			: Tag(tag) {}

		operator std::string&() { return Tag; }
		operator const char* () { return Tag.c_str(); }
	};

	struct MeshComponent
	{
	public:
		Ref<Atlas::Mesh> Mesh;
		MeshComponent() = default;
		MeshComponent(const std::string& path)
			: Mesh(CreateRef<Atlas::Mesh>(path)) {}

		operator Ref<Atlas::Mesh>& () { return Mesh; }
	};

}
