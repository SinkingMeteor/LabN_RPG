#include "Systems/SpriteRenderSystem.h"

namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registy, sf::RenderWindow& window)
	{
		auto view = registy.view<SpriteComponent, TransformComponent>();

		for (entt::entity entity : view)
		{
			SpriteComponent& spriteComponent = view.get<SpriteComponent>(entity);
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);

			spriteComponent.Sprite.setPosition(transformComponent.Position);
			spriteComponent.Sprite.setScale(transformComponent.Scale);
			spriteComponent.Sprite.setRotation(transformComponent.Rotation);

			window.draw(spriteComponent.Sprite);
		}
	}
}

