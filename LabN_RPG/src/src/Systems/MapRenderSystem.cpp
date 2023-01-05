#include "Systems/MapRenderSystem.h"

namespace vg 
{
	void MapRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		auto view = registry.view<TileMapComponent>();

		for (entt::entity entity : view)
		{
			TileMapComponent& mapComponent = view.get<TileMapComponent>(entity);
			sf::RenderStates states{};
			states.texture = &mapComponent.RelatedTexture->Texture;
			window.draw(mapComponent.VertexArray, states);
		}
	}
}

