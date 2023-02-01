#pragma once
#include "vgpch.h"
namespace vg 
{
	struct TransformComponent 
	{
	public:
		sf::Transform LocalTransform{};
		sf::Transform GlobalTransform{};
	};
}