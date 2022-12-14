#pragma once
#include "SFML/Graphics.hpp"
namespace vg 
{
	struct TransformComponent 
	{
	public:
		sf::Vector2f Position{};
		sf::Vector2f Velocity{};
		sf::Vector2f Scale{};
		float Rotation{};
	};
}