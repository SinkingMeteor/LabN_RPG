#pragma once
#include "SFML/Graphics.hpp"
namespace vg 
{
	struct DirtyTransformComponent 
	{
		sf::Transform DeltaTransform{sf::Transform::Identity};
	};
}