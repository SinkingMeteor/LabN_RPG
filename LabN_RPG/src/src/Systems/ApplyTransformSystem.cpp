#include "Systems/ApplyTransformSystem.h"
#include <cassert>
namespace vg
{
	void ApplyTransformSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		entt::entity rootEntity = m_world->GetSceneRootEntity();
		sf::Transform rootTransform = sf::Transform::Identity;

		ApplyTransformToChildren(rootEntity, registry, rootTransform);

		registry.clear<DirtyTransform>();
	}

	void ApplyTransformSystem::ApplyTransformToChildren(entt::entity rootEntity, entt::registry& registry, sf::Transform& parentTransform)
	{
		TransformComponent& transformComponent = registry.get<TransformComponent>(rootEntity);
		NodeComponent& nodeComponent = registry.get<NodeComponent>(rootEntity);

		if (registry.all_of<DirtyTransform>(rootEntity)) 
		{
			transformComponent.GlobalTransform = parentTransform * transformComponent.LocalTransform;
		}

		if (registry.all_of<DrawableComponent>(rootEntity)) 
		{
			DrawableComponent& spriteComponent = registry.get<DrawableComponent>(rootEntity);
			TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteComponent.RectIndex];
			sf::Vector2f spritePosition = transformComponent.GlobalTransform * spriteRect.Pivot;

			spriteComponent.VertexArray[0].position = spritePosition - spriteRect.Pivot;
			spriteComponent.VertexArray[1].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
			spriteComponent.VertexArray[2].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
			spriteComponent.VertexArray[3].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };
		}

		for (entt::entity entity : nodeComponent.Children)
		{
			assert(registry.all_of<NodeComponent>(entity) && registry.all_of<TransformComponent>(entity));

			ApplyTransformToChildren(entity, registry, transformComponent.GlobalTransform);
		}
	}

}