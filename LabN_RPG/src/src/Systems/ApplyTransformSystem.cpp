#include "Systems/ApplyTransformSystem.h"
namespace vg
{
	void ApplyTransformSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<DrawableComponent, TransformComponent>();
		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);

			for (size_t i = 0; i < spriteComponent.VertexArray.getVertexCount(); i += 4)
			{
				const TextureRect& spriteRect = spriteComponent.Rects[i / 4];
				spriteComponent.VertexArray[i].position = transformComponent.Position - spriteRect.Pivot;
				spriteComponent.VertexArray[i + 1].position = transformComponent.Position - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
				spriteComponent.VertexArray[i + 2].position = transformComponent.Position - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
				spriteComponent.VertexArray[i + 3].position = transformComponent.Position - spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };
			
				//Todo: add rotation and scale
			}
		}
	}

}