#pragma once
#include "Systems/ISystem.h"
namespace vg 
{
	class PivotDebugRenderSystem final : public IRenderSystem 
	{
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	};
}