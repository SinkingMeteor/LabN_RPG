#pragma once
#include "Systems/ISystem.h"
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"
namespace vg 
{
	class ActorMovementSystem : public ISystem
	{
		virtual void Tick(entt::registry& registy, sf::Time deltaTime) override;
	};
}