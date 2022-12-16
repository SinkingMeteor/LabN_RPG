#include "Systems/ActorMovementSystem.h"

namespace vg 
{
	void ActorMovementSystem::Tick(entt::registry& registy, sf::Time deltaTime)
	{
		auto view = registy.view<TransformComponent, MovementComponent>();

		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			MovementComponent& movementComponent = view.get<MovementComponent>(entity);
			transformComponent.Position += movementComponent.Velocity * deltaTime.asSeconds();
		}
	}
}


