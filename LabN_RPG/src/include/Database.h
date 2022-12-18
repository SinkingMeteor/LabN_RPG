#pragma once
#include "entt/core/hashed_string.hpp"

namespace vg 
{
	namespace Database 
	{
		namespace Textures 
		{
			static constexpr unsigned int HERO_ATLAS = 'HERO';
		}

		namespace AnimConfigs 
		{

			static constexpr unsigned int HERO_ANIM_CONFIG = 'HERO';
		}

		namespace AnimTypes 
		{
			static constexpr entt::id_type IDLE_W = entt::hashed_string{ "IDLW" }.value();
			static constexpr entt::id_type IDLE_A = entt::hashed_string{ "IDLA" }.value();
			static constexpr entt::id_type IDLE_S = entt::hashed_string{ "IDLS" }.value();
			static constexpr entt::id_type IDLE_D = entt::hashed_string{ "IDLD" }.value();
		}

	}
}