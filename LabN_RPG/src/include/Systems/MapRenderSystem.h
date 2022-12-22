#pragma once
#include "Systems/ISystem.h"
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include "Components/MapComponent.h"
namespace vg 
{
	class MapRenderSystem : public IRenderSystem 
	{
	public:
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	};
}