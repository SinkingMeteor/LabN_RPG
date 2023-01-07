#include "Systems/SpriteRenderSystem.h"
#include "MathUtils.h"
namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		registry.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs)
		{
			return (lhs.Transform * lhs.Origin).y < (rhs.Transform * rhs.Origin).y;
		});

		auto groundView = registry.view<TransformComponent, DrawableComponent, GroundSortingLayer>();
		DrawLayer(groundView, window);
		auto onGroundView = registry.view<TransformComponent, DrawableComponent, OnGroundSortingLayer>().use<TransformComponent>();
		DrawLayer(onGroundView, window);

	}

	void SpriteRenderSystem::DrawLayer(auto& view, sf::RenderWindow& window)
	{
		for (entt::entity entity : view)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			sf::RenderStates states{};
			states.texture = &spriteComponent.RelatedTexture->Texture;

			for (size_t i = 0; i < spriteComponent.VertexArray.getVertexCount(); i += 4)
			{
				const int spriteRectIndex = spriteComponent.RectsIndices[i / 4];
				if (spriteRectIndex < 0) continue;
				const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteRectIndex];
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

