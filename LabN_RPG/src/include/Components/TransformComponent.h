#pragma once
#include "SFML/Graphics.hpp"
namespace vg 
{
	struct TransformComponent 
	{
	public:
		sf::Transform Transform{};
		sf::Vector2f Origin{};
	};
}