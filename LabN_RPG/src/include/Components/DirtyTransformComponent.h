#pragma once
#include "vgpch.h"
namespace vg 
{
	struct DirtyTransformComponent 
	{
		sf::Transform DeltaTransform{sf::Transform::Identity};
	};
}