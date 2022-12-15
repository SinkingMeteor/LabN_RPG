#pragma once
#include "entt/entt.hpp"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/MovementComponent.h"
#include "SFML/Graphics.hpp"
namespace vg 
{
	class ActorPrefab 
	{
	public:
		static entt::entity CreateEntity(entt::registry& registry);
	};
}