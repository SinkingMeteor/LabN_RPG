#include "Systems/MapRenderSystem.h"

namespace vg 
{
	void MapRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		auto view = registry.view<MapComponent>();

		for (entt::entity entity : view)
		{
			MapComponent& mapComponent = view.get<MapComponent>(entity);
			sf::RenderStates states{};
			states.texture = mapComponent.RelatedTexture;
			window.draw(mapComponent.VertexArray, states);
		}
	}
}

