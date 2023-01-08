#pragma once
#include "entt/core/hashed_string.hpp"

namespace vg 
{
	namespace Database 
	{
		namespace Textures 
		{
			static constexpr unsigned int HERO_ATLAS = 'HERO';
			static constexpr unsigned int DESERT_MAP = 'MDES';
			static constexpr unsigned int FOREST_MAP = 'MFOR';
			static constexpr entt::id_type DESERT_GROUND_TILESET = entt::hashed_string{ "DesertGroundTiles" }.value();
			static constexpr entt::id_type FOREST_GROUND_TILESET = entt::hashed_string{ "ForestMap" }.value();
		}

		namespace AnimConfigs 
		{
			static constexpr unsigned int HERO_ANIM_CONFIG = 'HERO';
		}

		namespace AnimTypes 
		{
			static constexpr entt::id_type G_IDLE = entt::hashed_string{ "IDLE" }.value();
			static constexpr entt::id_type G_MOVE = entt::hashed_string{ "MOVE" }.value();

			static constexpr entt::id_type IDLE_W = entt::hashed_string{ "IDLW" }.value();
			static constexpr entt::id_type IDLE_A = entt::hashed_string{ "IDLA" }.value();
			static constexpr entt::id_type IDLE_S = entt::hashed_string{ "IDLS" }.value();
			static constexpr entt::id_type IDLE_D = entt::hashed_string{ "IDLD" }.value();

			static constexpr entt::id_type MOVE_W = entt::hashed_string{ "MOVW" }.value();
			static constexpr entt::id_type MOVE_A = entt::hashed_string{ "MOVA" }.value();
			static constexpr entt::id_type MOVE_S = entt::hashed_string{ "MOVS" }.value();
			static constexpr entt::id_type MOVE_D = entt::hashed_string{ "MOVD" }.value();
		}

		namespace SortingLayers 
		{
			static constexpr entt::id_type Ground = entt::hashed_string{ "Ground" }.value();
			static constexpr entt::id_type OnGround = entt::hashed_string{ "OnGround" }.value();
			static constexpr entt::id_type AboveGround = entt::hashed_string{ "AboveGround" }.value();
		}
	}
}