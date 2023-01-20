#pragma once
#include "Systems/ISystem.h"
#include "World/World.h"
namespace vg 
{
	class CollisionSystem final : public System 
	{
	public:
		CollisionSystem(World* world) : System(world) {}
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}