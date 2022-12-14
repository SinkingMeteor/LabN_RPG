#pragma once
#include <cassert>
#include "Systems/ISystem.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Locator.h"
namespace vg 
{
	class SpriteRenderSystem final : public IRenderSystem 
	{
	public:
		virtual void Render(entt::registry& registy, sf::RenderWindow& window) override;
	};
}