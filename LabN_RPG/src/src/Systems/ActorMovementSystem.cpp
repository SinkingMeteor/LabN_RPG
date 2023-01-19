#include "Systems/ActorMovementSystem.h"
#include <limits>
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
				entt::entity rootEntity = m_world->GetSceneRootEntity();
				TransformComponent& rootTransform = registry.get<TransformComponent>(rootEntity);
				WorldPartitionComponent& partitionGrid = registry.get<WorldPartitionComponent>(rootEntity);

				sf::Vector2f beforeGlobalPosition = rootTransform.GlobalTransform * transformComponent.LocalTransform * VGMath::Zero;
				PartitionCell& beforeCell = partitionGrid.Grid.GetCell(beforeGlobalPosition);

				transformComponent.LocalTransform.translate(movementComponent.Velocity * movementComponent.Speed * deltaTime.asSeconds());

				sf::Vector2f afterGlobalPosition = rootTransform.GlobalTransform * transformComponent.LocalTransform * VGMath::Zero;
				PartitionCell& afterCell = partitionGrid.Grid.GetCell(afterGlobalPosition);

				if (&beforeCell != &afterCell) 
				{
					beforeCell.RemoveEntity(entity);
					afterCell.AddEntity(entity);
				}

				registry.emplace<DirtyTransform>(entity);
			}
		}
	}
}


