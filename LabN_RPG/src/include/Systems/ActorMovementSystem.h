#pragma once
#include "Systems/ISystem.h"
#include "Components/Components.h"
namespace vg 
{
	class ActorMovementSystem : public System
	{
	public:
		ActorMovementSystem(World* world) : System(world) {}
	private:
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}