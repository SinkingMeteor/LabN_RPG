#pragma once
#include "SFML/Graphics.hpp"
namespace vg 
{
	struct TransformComponent 
	{
	public:
		sf::Transform LocalTransform{sf::Transform::Identity};
		sf::Transform GlobalTransform{ sf::Transform::Identity};
	};
}