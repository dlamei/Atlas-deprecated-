#pragma once

#include <string>
#include <bitset>


namespace ECS {
	using Entity = uint32_t;
	static const Entity MAX_ENTITIES = 5000;

	using ComponentType = uint8_t;
	static const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
}


