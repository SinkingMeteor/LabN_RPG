#pragma once
#include "Systems/ISystem.h"
#include "entt/entt.hpp"
#include "Components/Components.h"
#include "Database.h"
namespace vg 
{
	class PlayerControllerSystem final : public System 
	{
	public:
		PlayerControllerSystem(World* world) : System(world) {}
	private:
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}