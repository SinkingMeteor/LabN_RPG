#include "Systems/SpriteRenderSystem.h"

namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		auto view = registry.view<SpriteComponent, TransformComponent, AnimationComponent>();

		for (entt::entity entity : view)
		{
			SpriteComponent& spriteComponent = view.get<SpriteComponent>(entity);
			AnimationComponent& animationComponent = view.get<AnimationComponent>(entity);
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);

			spriteComponent.Sprite.setTextureRect(animationComponent.CurrentAnimation->Frames[animationComponent.CurrentFrame]);

			spriteComponent.Sprite.setPosition(transformComponent.Position);
			spriteComponent.Sprite.setScale(transformComponent.Scale);
			spriteComponent.Sprite.setRotation(transformComponent.Rotation);

			window.draw(spriteComponent.Sprite);
		}
	}
}

