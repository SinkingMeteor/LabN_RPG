#pragma once
#include "Components/Components.h"
#include "Systems/ISystem.h"
#include "entt/entt.hpp"

namespace vg 
{
	class ApplyTransformSystem final : public System
	{
	public:
		ApplyTransformSystem(World* world) : System(world) {}
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}