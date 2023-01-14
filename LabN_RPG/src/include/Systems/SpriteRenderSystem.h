#pragma once
#include <cassert>
#include "Systems/ISystem.h"
#include "Components/Components.h"
#include "entt/entt.hpp"
namespace vg 
{
	class SpriteRenderSystem final : public IRenderSystem 
	{
		struct PivotEntity 
		{
		public:
			entt::entity Entity{entt::null};
			float PivotY{0.0f};
		};

	public:
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	private:
		void DrawLayer(std::vector<PivotEntity>& sortedPivots, sf::RenderWindow& window, auto& view);
		void SortEntitiesByPivot(auto& view, std::vector<PivotEntity>& result);
	};
}