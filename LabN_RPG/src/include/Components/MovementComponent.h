#pragma once
#include "SFML/Graphics.hpp"

namespace vg
{
	struct MovementComponent
	{
		sf::Vector2f Velocity{};
		float Speed{};
	};
}
