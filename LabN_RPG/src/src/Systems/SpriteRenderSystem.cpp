#include "Systems/SpriteRenderSystem.h"
#include "MathUtils.h"
#include <algorithm>
namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		ProcessLayer<GroundSortingLayer>(registry, window);
		ProcessLayer<OnGroundSortingLayer>(registry, window);
		ProcessLayer<AboveGroundSortingLayer>(registry, window);
		registry.clear<ReadyForRenderThisFrame>();
	}

	void SpriteRenderSystem::CullTiles(auto& view, sf::RenderWindow& window, entt::registry& registry)
	{
		const sf::View& camera = window.getView();
		sf::Vector2f cameraViewportSize = camera.getSize();
		sf::Vector2f cameraViewportPosition = camera.getCenter() - cameraViewportSize / 2.0f;
		sf::FloatRect cameraViewportRect{ cameraViewportPosition, cameraViewportSize };

		for (entt::entity entity : view) 
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);

			const sf::Vector2f leftTopVertexPosition = spriteComponent.VertexArray[0].position;
			const sf::Vector2f rightBottomVertexPosition = spriteComponent.VertexArray[2].position;
			const sf::FloatRect tileRect = VGMath::TwoVectorsToRect(leftTopVertexPosition, rightBottomVertexPosition);

			if (!VGMath::Intersects2D(tileRect, cameraViewportRect))
			{
				continue;
			}

			registry.emplace<ReadyForRenderThisFrame>(entity);
		}
	}

	void SpriteRenderSystem::SortEntitiesByPivot(auto& view, std::vector<PivotEntity>& result)
	{
		std::vector<PivotEntity> unsortablePivots{};

		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			DrawableComponent& drawableComponent = view.get<DrawableComponent>(entity);

			TextureRect& rect = drawableComponent.RelatedTexture->RectDatas[drawableComponent.RectIndex];
			sf::Vector2f pivotPosition = transformComponent.GlobalTransform * rect.Pivot;
			PivotEntity pivotEntity{ entity, pivotPosition.y };
			result.push_back(pivotEntity);
		}

		std::sort(result.begin(), result.end(), [](PivotEntity& lhs, PivotEntity& rhs) { return lhs.PivotY < rhs.PivotY; });
	}

	void SpriteRenderSystem::DrawLayer(std::vector<PivotEntity>& sortedPivots, sf::RenderWindow& window, auto& view)
	{
		std::vector<sf::Vertex> verticesToDraw{};
		verticesToDraw.reserve(sortedPivots.size());
		sf::RenderStates states{};
		for (PivotEntity& sortedEntity : sortedPivots)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(sortedEntity.Entity);
			sf::Texture* texture = &spriteComponent.RelatedTexture->Texture;

			if (states.texture != nullptr && states.texture != texture) 
			{
				window.draw(verticesToDraw.data(), verticesToDraw.size(), sf::Quads, states);
				verticesToDraw.clear();
			}

			states.texture = texture;

			const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteComponent.RectIndex];
			float top = (float)spriteRect.Rect.top;
			float left = (float)spriteRect.Rect.left;
			float width = (float)spriteRect.Rect.width;
			float height = (float)spriteRect.Rect.height;

			spriteComponent.VertexArray[0].texCoords = sf::Vector2f{ left, top };
			spriteComponent.VertexArray[1].texCoords = sf::Vector2f{ left + width, top };
			spriteComponent.VertexArray[2].texCoords = sf::Vector2f{ left + width, top + height };
			spriteComponent.VertexArray[3].texCoords = sf::Vector2f{ left, top + height };

			verticesToDraw.push_back(spriteComponent.VertexArray[0]);
			verticesToDraw.push_back(spriteComponent.VertexArray[1]);
			verticesToDraw.push_back(spriteComponent.VertexArray[2]);
			verticesToDraw.push_back(spriteComponent.VertexArray[3]);
		}

		window.draw(verticesToDraw.data(), verticesToDraw.size(), sf::Quads, states);
	}
}

