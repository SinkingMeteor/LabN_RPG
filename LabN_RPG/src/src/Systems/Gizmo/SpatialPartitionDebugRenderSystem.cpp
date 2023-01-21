#include "Systems/Gizmo/SpatialPartitionDebugRenderSystem.h"
#include "Components/Components.h"
namespace vg 
{
	void SpatialPartitionDebugRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		WorldPartitionComponent& partitionComponent = registry.get<WorldPartitionComponent>(m_world->GetSceneRootEntity());
		sf::Vector2<std::size_t> gridSize = partitionComponent.Grid.GetGridSize();
		float cellSize = partitionComponent.Grid.GetCellSize();
		auto actorView = registry.view<CameraTarget>();
		auto colliderView = registry.view<RectColliderComponent>();

		for (size_t i = 0; i < gridSize.x * gridSize.y; ++i)
		{
			std::size_t xPos = i % gridSize.x;
			std::size_t yPos = i / gridSize.x;

			sf::Vector2f rectPosition = sf::Vector2f{ (float)cellSize * (float)xPos, (float)cellSize * (float)yPos };
			sf::RectangleShape rect{ sf::Vector2f{cellSize, cellSize} };
			rect.setPosition(rectPosition);
			rect.setFillColor(sf::Color::Transparent);

			PartitionCell& cell = partitionComponent.Grid.GetCell(rectPosition);

			for (entt::entity actor : actorView)
			{
				if (cell.Contains(actor))
				{
					rect.setOutlineColor(sf::Color::Cyan);
				}
				else
				{
					rect.setOutlineColor(sf::Color::Magenta);
				}
			}

			for (entt::entity collider : colliderView)
			{
				if (cell.Contains(collider))
				{
					rect.setOutlineColor(sf::Color::Red);
				}
			}

			rect.setOutlineThickness(-1.5f);
			window.draw(rect);
		}
	}
}