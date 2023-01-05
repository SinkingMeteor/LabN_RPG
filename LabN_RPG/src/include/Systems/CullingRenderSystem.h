#pragma once
#include "Systems/ISystem.h"
namespace vg 
{
	class CullingRenderSystem : IRenderSystem 
	{
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	};
}