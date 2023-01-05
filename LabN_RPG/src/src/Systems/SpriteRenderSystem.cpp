#include "Systems/SpriteRenderSystem.h"

namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		registry.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs) 
		{
			return lhs.Position.y < rhs.Position.y;
		});

		auto view = registry.view<TransformComponent, DrawableComponent, OnGroundSortingLayer>();

		for (entt::entity entity : view)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			sf::RenderStates states{};
			states.texture = &spriteComponent.RelatedTexture->Texture;

			for (size_t i = 0; i < spriteComponent.VertexArray.getVertexCount(); i += 4)
			{
				const TextureRect& spriteRect = spriteComponent.Rects[i / 4];
			
				float top = (float)spriteRect.Rect.top;
				float left = (float)spriteRect.Rect.left;
				float width = (float)spriteRect.Rect.width;
				float height = (float)spriteRect.Rect.height;

				spriteComponent.VertexArray[i].texCoords = sf::Vector2f{ left, top };
				spriteComponent.VertexArray[i + 1].texCoords = sf::Vector2f{ left + width, top };
				spriteComponent.VertexArray[i + 2].texCoords = sf::Vector2f{ left + width, top + height };
				spriteComponent.VertexArray[i + 3].texCoords = sf::Vector2f{ left, top + height };
			}


			window.draw(spriteComponent.VertexArray, states);
		}
	}

}

