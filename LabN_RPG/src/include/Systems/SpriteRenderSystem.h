#pragma once
#include <cassert>
#include "Systems/ISystem.h"
#include "Components/Components.h"
namespace vg 
{
	class SpriteRenderSystem final : public IRenderSystem 
	{
	public:
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	};
}