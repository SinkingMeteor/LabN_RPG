#pragma once
#include "vgpch.h"

namespace vg
{
	struct MovementComponent
	{
		sf::Vector2f FacingDirection{};
		sf::Vector2f Velocity{};
		float Speed{};
	};
}
