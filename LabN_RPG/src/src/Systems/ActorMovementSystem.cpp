#include "Systems/ActorMovementSystem.h"
#include "MathUtils.h"
namespace vg 
{
	void ActorMovementSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<TransformComponent, MovementComponent>();

		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			MovementComponent& movementComponent = view.get<MovementComponent>(entity);
			if (VGMath::GetSqrtLengthOfVector2(movementComponent.Velocity) > FLT_EPSILON) 
			{
				DirtyTransformComponent& dirtyTransformComponent = registry.emplace<DirtyTransformComponent>(entity);
				dirtyTransformComponent.DeltaTransform.translate(movementComponent.Velocity * movementComponent.Speed * deltaTime.asSeconds());
			}
		}
	}
}


