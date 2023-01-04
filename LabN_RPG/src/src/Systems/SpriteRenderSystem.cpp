#include "Systems/SpriteRenderSystem.h"

namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		registry.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs) 
		{
			return lhs.Position.y < rhs.Position.y;
		});

		auto view = registry.view<TransformComponent, SpriteComponent, OnGroundSortingLayer>();

		for (entt::entity entity : view)
		{
			SpriteComponent& spriteComponent = view.get<SpriteComponent>(entity);
			window.draw(spriteComponent.Sprite);
		}
	}

}

