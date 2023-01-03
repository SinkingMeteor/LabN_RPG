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

			const std::size_t frameIndex = animationComponent.CurrentAnimation->Frames[animationComponent.CurrentFrame];
			const TextureRect& textureRect = animationComponent.CurrentAnimation->Texture->RectDatas[frameIndex];
			const sf::IntRect& rect = textureRect.Rect;
			spriteComponent.Sprite.setOrigin(textureRect.Pivot);
			spriteComponent.Sprite.setTextureRect(rect);
			spriteComponent.Sprite.setOrigin(sf::Vector2f{ rect.width * 0.5f, rect.height * 0.5f });
			
			spriteComponent.Sprite.setPosition(transformComponent.Position);
			spriteComponent.Sprite.setScale(transformComponent.Scale);
			spriteComponent.Sprite.setRotation(transformComponent.Rotation);

			window.draw(spriteComponent.Sprite);
		}
	}
}

