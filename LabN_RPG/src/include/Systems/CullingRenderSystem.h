#pragma once
#include "Systems/ISystem.h"
namespace vg 
{
	class CullingRenderSystem final : public IRenderSystem 
	{
	public:
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	};
}