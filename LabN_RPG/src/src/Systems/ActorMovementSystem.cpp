#include "Systems/ActorMovementSystem.h"

namespace vg 
{
	void ActorMovementSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<TransformComponent, MovementComponent>();

		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			MovementComponent& movementComponent = view.get<MovementComponent>(entity);
			transformComponent.LocalTransform.translate(movementComponent.Velocity * movementComponent.Speed * deltaTime.asSeconds());
		}
	}
}


