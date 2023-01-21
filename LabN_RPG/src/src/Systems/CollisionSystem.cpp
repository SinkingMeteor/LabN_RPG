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
		entt::entity sceneRootEntity = m_world->GetSceneRootEntity();
		WorldPartitionComponent& partitionComponent = registry.get<WorldPartitionComponent>(sceneRootEntity);

		const std::vector<PartitionCell>& partitionCells = partitionComponent.Grid.GetCellArray();

		for (size_t partitionCellIndex = 0; partitionCellIndex < partitionCells.size(); ++partitionCellIndex)
		{
			const std::vector<entt::entity>& entitiesInTargetCell = partitionCells[partitionCellIndex].GetEntities();

			for (size_t entityInCellIndex = 0; entityInCellIndex < entitiesInTargetCell.size(); ++entityInCellIndex)
			{
				entt::entity entityToCheck = entitiesInTargetCell[entityInCellIndex];

				if (!registry.all_of<DirtyTransformComponent, RectColliderComponent>(entityToCheck)) continue;

				TransformComponent& entityTransform = registry.get<TransformComponent>(entityToCheck);
				DirtyTransformComponent& entityDirtyTransform = registry.get<DirtyTransformComponent>(entityToCheck);
				RectColliderComponent& entityCollider = registry.get<RectColliderComponent>(entityToCheck);

				sf::Vector2f entityColliderLocalPosition = sf::Vector2f{ entityCollider.Rect.left, entityCollider.Rect.top };
				sf::Vector2f entityCurrentPosition = entityTransform.GlobalTransform * entityDirtyTransform.DeltaTransform * entityColliderLocalPosition;
				sf::Vector2f entityColliderSize{ entityCollider.Rect.width, entityCollider.Rect.height };
				sf::FloatRect entityColliderRect{ entityCurrentPosition, entityColliderSize };

				std::vector<std::size_t> neighbourCellIndices{};
				partitionComponent.Grid.GetAllCellsAround(partitionCellIndex, neighbourCellIndices);
				
				bool intersects = false;
				for (size_t neighbourCellIndex = 0; neighbourCellIndex < neighbourCellIndices.size(); ++neighbourCellIndex)
				{
					if (intersects) break;

					const std::vector<entt::entity>& entitiesInNeighbourCell = partitionCells[neighbourCellIndices[neighbourCellIndex]].GetEntities();

					for (size_t neighbourEntityIndex = 0; neighbourEntityIndex < entitiesInNeighbourCell.size(); ++neighbourEntityIndex)
					{
						entt::entity entityNeighbour = entitiesInNeighbourCell[neighbourEntityIndex];

						if (!registry.all_of<RectColliderComponent>(entityNeighbour) || entityNeighbour == entityToCheck) continue;

						RectColliderComponent& neighbourCollider = registry.get<RectColliderComponent>(entityNeighbour);
						TransformComponent& neighbourTransform = registry.get<TransformComponent>(entityNeighbour);

						sf::Vector2f neighbourColliderLocalPosition = sf::Vector2f{ neighbourCollider.Rect.left, neighbourCollider.Rect.top };
						sf::Vector2f neighbourCurrentPosition = neighbourTransform.GlobalTransform * neighbourColliderLocalPosition;
						sf::Vector2f neighbourColliderSize{ neighbourCollider.Rect.width, neighbourCollider.Rect.height };
						sf::FloatRect neighbourColliderRect{ neighbourCurrentPosition, neighbourColliderSize };

						if (VGMath::Intersects2D(entityColliderRect, neighbourColliderRect))
						{
							intersects = true;
							registry.remove<DirtyTransformComponent>(entityToCheck);
							break;
						}
					}
				}
			}
		}
	}
}