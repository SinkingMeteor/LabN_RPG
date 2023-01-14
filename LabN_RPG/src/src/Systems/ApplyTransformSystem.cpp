#include "Systems/ApplyTransformSystem.h"
#include <cassert>
namespace vg
{
	void ApplyTransformSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{

		entt::entity rootEntity = m_world->GetSceneRootEntity();
		sf::Transform rootTransform = sf::Transform::Identity;

		ApplyTransformToChildren(rootEntity, registry, rootTransform);

		/*
		auto view = registry.view<DrawableComponent, TransformComponent, NodeComponent>();

		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			std::size_t amountOfTiles = spriteComponent.VertexArray.getVertexCount() / 4;
			for (size_t i = 0; i < amountOfTiles; ++i)
			{
				const int spriteRectIndex = spriteComponent.RectsIndices[i];
				if (spriteRectIndex < 0) continue;
				const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteRectIndex];

				float x = i % spriteComponent.SpriteWidthByTiles;
				float y = i / spriteComponent.SpriteWidthByTiles;
				
				const sf::Vector2f gridOffset = sf::Vector2f{ x * spriteRect.Rect.width, y * spriteRect.Rect.width };

				sf::Vector2f spritePosition = transformComponent.Transform * spriteRect.Pivot;

				spriteComponent.VertexArray[i * 4].position = spritePosition - spriteRect.Pivot + gridOffset;
				spriteComponent.VertexArray[i * 4 + 1].position = spritePosition - spriteRect.Pivot + sf::Vector2f{(float)spriteRect.Rect.width, 0.0f} + gridOffset;
				spriteComponent.VertexArray[i * 4 + 2].position = spritePosition - spriteRect.Pivot + sf::Vector2f{(float)spriteRect.Rect.width, (float)spriteRect.Rect.height} + gridOffset;
				spriteComponent.VertexArray[i * 4 + 3].position = spritePosition - spriteRect.Pivot + sf::Vector2f{0.0f, (float)spriteRect.Rect.height} + gridOffset;
			}
		}*/
	}

	void ApplyTransformSystem::ApplyTransformToChildren(entt::entity rootEntity, entt::registry& registry, sf::Transform& parentTransform)
	{
		TransformComponent& transformComponent = registry.get<TransformComponent>(rootEntity);
		NodeComponent& nodeComponent = registry.get<NodeComponent>(rootEntity);

		transformComponent.GlobalTransform = parentTransform * transformComponent.LocalTransform;

		for (entt::entity entity : nodeComponent.Children)
		{
			assert(registry.all_of<NodeComponent>(entity) && registry.all_of<TransformComponent>(entity));

			ApplyTransformToChildren(entity, registry, transformComponent.GlobalTransform);
		}
	}

}