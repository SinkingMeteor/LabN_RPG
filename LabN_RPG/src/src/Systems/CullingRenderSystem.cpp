#include "Systems/CullingRenderSystem.h"
#include "Components/Components.h"
#include "Common/MathUtils.h"
namespace vg 
{
	void CullingRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		auto view = registry.view<TransformComponent, DrawableComponent>();

		const sf::View& camera = window.getView();
		sf::Vector2f cameraViewportSize = camera.getSize();
		sf::Vector2f cameraViewportPosition = camera.getCenter() - cameraViewportSize / 2.0f;
		sf::FloatRect cameraViewportRect{ cameraViewportPosition, cameraViewportSize };

		for (entt::entity entity : view)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			spriteComponent.CullingBits.reset();

			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			std::size_t amountOfTiles = spriteComponent.VertexArray.getVertexCount() / 4;
			
			for (size_t i = 0; i < amountOfTiles; ++i)
			{
				const int spriteRectIndex = spriteComponent.RectsIndices[i];
				if (spriteRectIndex < 0) continue;
				const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteRectIndex];

				float x = i % spriteComponent.SpriteWidthByTiles;
				float y = i / spriteComponent.SpriteWidthByTiles;

				const sf::Vector2f gridOffset = sf::Vector2f{ x * spriteRect.Rect.width, y * spriteRect.Rect.width };

				const sf::Vector2f leftTopVertexPosition = spriteComponent.VertexArray[i * 4].position;
				const sf::Vector2f rightBottomVertexPosition = spriteComponent.VertexArray[i * 4 + 2].position;
				const sf::FloatRect tileRect = VGMath::TwoVectorsToRect(leftTopVertexPosition, rightBottomVertexPosition);
				if (!VGMath::Intersects2D(tileRect, cameraViewportRect)) 
				{
					spriteComponent.CullingBits.set(i, true);
				}
			}
		}
	}
}