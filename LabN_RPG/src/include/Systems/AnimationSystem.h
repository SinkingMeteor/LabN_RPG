#pragma once
#include "World/World.h"
#include "Components/Components.h"
#include "Systems/ISystem.h"

namespace vg 
{
	class AnimationSystem final : public System 
	{
	public:
		AnimationSystem(World* world) : System(world) {};
	private:
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}