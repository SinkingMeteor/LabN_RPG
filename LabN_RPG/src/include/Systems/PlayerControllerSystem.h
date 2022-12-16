#pragma once
#include "Systems/ISystem.h"
#include "entt/entt.hpp"
#include "Components/PlayerControllerComponent.h"
#include "Components/MovementComponent.h"
#include "Components/CommonTags.h"
#include "Locator.h"
namespace vg 
{
	class PlayerControllerSystem : public ISystem 
	{
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}