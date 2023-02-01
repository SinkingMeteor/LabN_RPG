#include "Systems/ApplyTransformSystem.h"
#include "MathUtils.h"
namespace vg
{
	void ApplyTransformSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		entt::entity rootEntity = m_world->GetSceneRootEntity();
		sf::Transform rootTransform = sf::Transform::Identity;

		ApplyTransformToChildren(rootEntity, registry, rootTransform);

		registry.clear<DirtyTransformComponent>();
	}

	void ApplyTransformSystem::ApplyTransformToChildren(entt::entity rootEntity, entt::registry& registry, sf::Transform& parentTransform)
	{
		TransformComponent& transformComponent = registry.get<TransformComponent>(rootEntity);
		NodeComponent& nodeComponent = registry.get<NodeComponent>(rootEntity);

		if (registry.all_of<DirtyTransformComponent>(rootEntity)) 
		{
			entt::entity worldEntity = m_world->GetSceneRootEntity();
			WorldPartitionComponent& partitionGrid = registry.get<WorldPartitionComponent>(worldEntity);

			sf::Vector2f beforeGlobalPosition = transformComponent.GlobalTransform * VGMath::Zero;
			PartitionCell& beforeCell = partitionGrid.Grid.GetCell(beforeGlobalPosition);

			DirtyTransformComponent& deltaTransform = registry.get<DirtyTransformComponent>(rootEntity);
			transformComponent.LocalTransform = transformComponent.LocalTransform * deltaTransform.DeltaTransform;
			transformComponent.GlobalTransform = parentTransform * transformComponent.LocalTransform;

			sf::Vector2f afterGlobalPosition = transformComponent.GlobalTransform * VGMath::Zero;
			PartitionCell& afterCell = partitionGrid.Grid.GetCell(afterGlobalPosition);

			if (&beforeCell != &afterCell)
			{
				beforeCell.RemoveEntity(rootEntity);
				afterCell.AddEntity(rootEntity);
			}
		}

		if (registry.all_of<DrawableComponent>(rootEntity)) 
		{
			DrawableComponent& spriteComponent = registry.get<DrawableComponent>(rootEntity);
			TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteComponent.RectIndex];
			sf::Vector2f spritePosition = transformComponent.GlobalTransform * VGMath::Zero;

			spriteComponent.VertexArray[0].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };
			spriteComponent.VertexArray[1].position = spritePosition - spriteRect.Pivot;
			spriteComponent.VertexArray[2].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
		
			spriteComponent.VertexArray[3].position = spriteComponent.VertexArray[0].position;
			spriteComponent.VertexArray[4].position = spriteComponent.VertexArray[2].position;
			spriteComponent.VertexArray[5].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
		}

		for (entt::entity entity : nodeComponent.Children)
		{
			assert(registry.all_of<NodeComponent>(entity) && registry.all_of<TransformComponent>(entity));

			ApplyTransformToChildren(entity, registry, transformComponent.GlobalTransform);
		}
	}

}