#include "Systems/SpriteRenderSystem.h"
#include "MathUtils.h"
#include <algorithm>
namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		std::vector<PivotEntity> sortResult{};

		auto groundView = registry.view<TransformComponent, DrawableComponent, GroundSortingLayer>();
		SortEntitiesByPivot(groundView, sortResult);
		DrawLayer(sortResult, window, groundView);
		sortResult.clear();

		auto onGroundView = registry.view<TransformComponent, DrawableComponent, OnGroundSortingLayer>();
		SortEntitiesByPivot(onGroundView, sortResult);
		DrawLayer(sortResult, window, onGroundView);
		sortResult.clear();

		auto aboveGroundView = registry.view<TransformComponent, DrawableComponent, AboveGroundSortingLayer>();
		SortEntitiesByPivot(aboveGroundView, sortResult);
		DrawLayer(sortResult, window, aboveGroundView);

	}

	void SpriteRenderSystem::SortEntitiesByPivot(auto& view, std::vector<PivotEntity>& result)
	{
		for (entt::entity entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			DrawableComponent& drawableComponent = view.get<DrawableComponent>(entity);

			if (drawableComponent.RectsIndices.size() == 0) continue;

			TextureRect& rect = drawableComponent.RelatedTexture->RectDatas[0];
			sf::Vector2f pivotPosition = transformComponent.GlobalTransform * rect.Pivot;
			PivotEntity pivotEntity{ entity, pivotPosition.y };
			result.push_back(pivotEntity);
		}

		std::sort(result.begin(), result.end(), [](PivotEntity& lhs, PivotEntity& rhs) { return lhs.PivotY < rhs.PivotY; });
	}

	void SpriteRenderSystem::DrawLayer(std::vector<PivotEntity>& sortedPivots, sf::RenderWindow& window, auto& view)
	{
		const sf::View& camera = window.getView();
		sf::Vector2f cameraViewportSize = camera.getSize();
		sf::Vector2f cameraViewportPosition = camera.getCenter() - cameraViewportSize / 2.0f;
		sf::FloatRect cameraViewportRect{ cameraViewportPosition, cameraViewportSize };

		for (PivotEntity& sortedEntity : sortedPivots)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(sortedEntity.Entity);
			TransformComponent& transformComponent = view.get<TransformComponent>(sortedEntity.Entity);
			sf::RenderStates states{};
			states.texture = &spriteComponent.RelatedTexture->Texture;

			std::size_t amountOfTiles = spriteComponent.VertexArray.getVertexCount() / 4;

			//todo: solve copy problem
			std::vector<sf::Vertex> verticesToDraw{};
			verticesToDraw.reserve(amountOfTiles);

			for (size_t i = 0; i < amountOfTiles; ++i)
			{
				const int spriteRectIndex = spriteComponent.RectsIndices[i];
				if (spriteRectIndex < 0) continue;

				const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteRectIndex];
				float top = (float)spriteRect.Rect.top;
				float left = (float)spriteRect.Rect.left;
				float width = (float)spriteRect.Rect.width;
				float height = (float)spriteRect.Rect.height;

				float x = i % spriteComponent.SpriteWidthByTiles;
				float y = i / spriteComponent.SpriteWidthByTiles;

				const sf::Vector2f gridOffset = sf::Vector2f{ x * spriteRect.Rect.width, y * spriteRect.Rect.width };

				sf::Vector2f spritePosition = transformComponent.GlobalTransform * (gridOffset + spriteRect.Pivot);

				spriteComponent.VertexArray[i * 4].position = spritePosition - spriteRect.Pivot;
				spriteComponent.VertexArray[i * 4 + 1].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
				spriteComponent.VertexArray[i * 4 + 2].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
				spriteComponent.VertexArray[i * 4 + 3].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };

				const sf::Vector2f leftTopVertexPosition = spriteComponent.VertexArray[i * 4].position;
				const sf::Vector2f rightBottomVertexPosition = spriteComponent.VertexArray[i * 4 + 2].position;
				const sf::FloatRect tileRect = VGMath::TwoVectorsToRect(leftTopVertexPosition, rightBottomVertexPosition);

				if (!VGMath::Intersects2D(tileRect, cameraViewportRect))
				{
					continue;
				}

				spriteComponent.VertexArray[i * 4].texCoords = sf::Vector2f{ left, top };
				spriteComponent.VertexArray[i * 4 + 1].texCoords = sf::Vector2f{ left + width, top };
				spriteComponent.VertexArray[i * 4 + 2].texCoords = sf::Vector2f{ left + width, top + height };
				spriteComponent.VertexArray[i * 4 + 3].texCoords = sf::Vector2f{ left, top + height };
			
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 1]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 2]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 3]);
			}
			if (verticesToDraw.size() > 0)
			{
				window.draw(verticesToDraw.data(), verticesToDraw.size(), sf::Quads, states);
			}
		}
	}

	void SpriteRenderSystem::DrawLayer(auto& view, sf::RenderWindow& window)
	{
		const sf::View& camera = window.getView();
		sf::Vector2f cameraViewportSize = camera.getSize();
		sf::Vector2f cameraViewportPosition = camera.getCenter() - cameraViewportSize / 2.0f;
		sf::FloatRect cameraViewportRect{ cameraViewportPosition, cameraViewportSize };

		for (entt::entity entity : view)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			sf::RenderStates states{};
			states.texture = &spriteComponent.RelatedTexture->Texture;

			std::size_t amountOfTiles = spriteComponent.VertexArray.getVertexCount() / 4;

			//todo: solve copy problem
			std::vector<sf::Vertex> verticesToDraw{};
			verticesToDraw.reserve(amountOfTiles);

			for (size_t i = 0; i < amountOfTiles; ++i)
			{
				const int spriteRectIndex = spriteComponent.RectsIndices[i];
				if (spriteRectIndex < 0) continue;

				const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteRectIndex];
				float top = (float)spriteRect.Rect.top;
				float left = (float)spriteRect.Rect.left;
				float width = (float)spriteRect.Rect.width;
				float height = (float)spriteRect.Rect.height;

				float x = i % spriteComponent.SpriteWidthByTiles;
				float y = i / spriteComponent.SpriteWidthByTiles;

				const sf::Vector2f gridOffset = sf::Vector2f{ x * spriteRect.Rect.width, y * spriteRect.Rect.width };

				sf::Vector2f spritePosition = transformComponent.GlobalTransform * (gridOffset + spriteRect.Pivot);

				spriteComponent.VertexArray[i * 4].position = spritePosition - spriteRect.Pivot;
				spriteComponent.VertexArray[i * 4 + 1].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, 0.0f };
				spriteComponent.VertexArray[i * 4 + 2].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ (float)spriteRect.Rect.width, (float)spriteRect.Rect.height };
				spriteComponent.VertexArray[i * 4 + 3].position = spritePosition - spriteRect.Pivot + sf::Vector2f{ 0.0f, (float)spriteRect.Rect.height };

				const sf::Vector2f leftTopVertexPosition = spriteComponent.VertexArray[i * 4].position;
				const sf::Vector2f rightBottomVertexPosition = spriteComponent.VertexArray[i * 4 + 2].position;
				const sf::FloatRect tileRect = VGMath::TwoVectorsToRect(leftTopVertexPosition, rightBottomVertexPosition);

				if (!VGMath::Intersects2D(tileRect, cameraViewportRect))
				{
					continue;
				}

				spriteComponent.VertexArray[i * 4].texCoords = sf::Vector2f{ left, top };
				spriteComponent.VertexArray[i * 4 + 1].texCoords = sf::Vector2f{ left + width, top };
				spriteComponent.VertexArray[i * 4 + 2].texCoords = sf::Vector2f{ left + width, top + height };
				spriteComponent.VertexArray[i * 4 + 3].texCoords = sf::Vector2f{ left, top + height };

				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 1]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 2]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 3]);
			}
			if (verticesToDraw.size() > 0)
			{
				window.draw(verticesToDraw.data(), verticesToDraw.size(), sf::Quads, states);
			}
		}
	}

}

