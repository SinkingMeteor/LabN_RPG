#pragma once 
#include "entt/entt.hpp"
#include "entt/core/hashed_string.hpp"
#include "SFML/Graphics.hpp"
#include "MathUtils.h"
#include <string>
namespace vg 
{
	class GameplayUtils 
	{
	public:
		static entt::id_type GetIDByDirection(const std::string& textWithNoPrefix, const sf::Vector2f& direction);
	};
}