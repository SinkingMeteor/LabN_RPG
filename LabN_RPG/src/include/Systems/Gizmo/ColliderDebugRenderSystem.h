#pragma once
#include "Systems/ISystem.h"
namespace vg 
{
	class ColliderDebugRenderSystem final : public IRenderSystem 
	{
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	};
}