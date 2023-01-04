#include "Systems/ApplyTransformSystem.h"
namespace vg
{
	void ApplyTransformSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<SpriteComponent, TransformComponent>();
		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			SpriteComponent& spriteComponent = view.get<SpriteComponent>(entity);

			spriteComponent.Sprite.setPosition(transformComponent.Position);
			spriteComponent.Sprite.setScale(transformComponent.Scale);
			spriteComponent.Sprite.setRotation(transformComponent.Rotation);
		}
	}

}