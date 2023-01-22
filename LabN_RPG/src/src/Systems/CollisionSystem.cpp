#include "Systems/CollisionSystem.h"
#include "Components/Components.h"
#include <vector>
#include "entt/entt.hpp"
#include "Common/MathUtils.h"
#include "Common/GameplayUtils.h"
#include <chrono>
namespace vg 
{
	void CollisionSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		registry.clear<Triggered>();

		entt::entity sceneRootEntity = m_world->GetSceneRootEntity();
		WorldPartitionComponent& partitionComponent = registry.get<WorldPartitionComponent>(sceneRootEntity);

		const std::vector<PartitionCell>& partitionCells = partitionComponent.Grid.GetCellArray();

		auto collidersView = registry.view<TransformComponent, RectColliderComponent>();

		for (entt::entity colliderEntity : collidersView)
		{
			TransformComponent& entityTransform = registry.get<TransformComponent>(colliderEntity);
			RectColliderComponent& entityCollider = registry.get<RectColliderComponent>(colliderEntity);

			sf::Vector2f entityColliderLocalPosition = sf::Vector2f{ entityCollider.Rect.left, entityCollider.Rect.top };
			sf::Vector2f entityCurrentPosition{};
			
			if (registry.all_of<DirtyTransformComponent>(colliderEntity)) 
			{
				DirtyTransformComponent& colliderDirtyTransform = registry.get<DirtyTransformComponent>(colliderEntity);
				entityCurrentPosition = entityTransform.GlobalTransform * colliderDirtyTransform.DeltaTransform * entityColliderLocalPosition;
			}
			else
			{
				entityCurrentPosition = entityTransform.GlobalTransform * entityColliderLocalPosition;
			}

			sf::Vector2f entityColliderSize{ entityCollider.Rect.width, entityCollider.Rect.height };
			sf::FloatRect entityColliderRect{ entityCurrentPosition, entityColliderSize };

			std::size_t currentCellIndex = partitionComponent.Grid.GetCellIndexByPosition(entityCurrentPosition);
			std::vector<std::size_t> neighbourCellIndices{};
			partitionComponent.Grid.GetAllCellsAround(currentCellIndex, neighbourCellIndices);

			bool intersects = false;

			for (size_t cellIndex : neighbourCellIndices)
			{
				if (intersects) break;

				const std::vector<entt::entity>& entitiesInNeighbourCell = partitionCells[cellIndex].GetEntities();
				
				for (entt::entity entityNeighbour : entitiesInNeighbourCell)
				{
					if (!registry.all_of<RectColliderComponent>(entityNeighbour) || entityNeighbour == colliderEntity) continue;

					RectColliderComponent& neighbourCollider = registry.get<RectColliderComponent>(entityNeighbour);
					TransformComponent& neighbourTransform = registry.get<TransformComponent>(entityNeighbour);

					sf::Vector2f neighbourColliderLocalPosition = sf::Vector2f{ neighbourCollider.Rect.left, neighbourCollider.Rect.top };
					sf::Vector2f neighbourCurrentPosition = neighbourTransform.GlobalTransform * neighbourColliderLocalPosition;
					sf::Vector2f neighbourColliderSize{ neighbourCollider.Rect.width, neighbourCollider.Rect.height };
					sf::FloatRect neighbourColliderRect{ neighbourCurrentPosition, neighbourColliderSize };

					if (!VGMath::Intersects2D(entityColliderRect, neighbourColliderRect)) continue;
				
					intersects = true;

					registry.emplace<Triggered>(colliderEntity);

					if (registry.all_of<DirtyTransformComponent>(colliderEntity) && !registry.all_of<Trigger>(entityNeighbour))
					{
						registry.remove<DirtyTransformComponent>(colliderEntity);
					}

					break;
				}
			}
		}
	}
}