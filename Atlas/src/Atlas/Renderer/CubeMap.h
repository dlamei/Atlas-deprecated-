#pragma once

#include "Atlas/Core/Core.h"
#include "VertexArray.h"

namespace Atlas {

	class CubeMap
	{
	private:
		Ref<VertexArray> m_VertexArray;

	public:

		CubeMap();

		void Load();

		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }
	};

}

