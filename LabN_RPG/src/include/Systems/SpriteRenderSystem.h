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
		void CullTiles(auto& view, sf::RenderWindow& window, entt::registry& registry);
		void SortEntitiesByPivot(auto& view, std::vector<PivotEntity>& result);

		template<typename SortingLayerType>
		void ProcessLayer(entt::registry& registry, sf::RenderWindow& window);
	};

	template<typename SortingLayerType>
	inline void SpriteRenderSystem::ProcessLayer(entt::registry& registry, sf::RenderWindow& window)
	{
		std::vector<PivotEntity> sortResult{};

		auto groundView = registry.view<DrawableComponent, SortingLayerType>();
		CullTiles(groundView, window, registry);
		auto renderableGroundView = registry.view<TransformComponent, DrawableComponent, SortingLayerType, ReadyForRenderThisFrame>();
		
		//?
		sortResult.reserve(renderableGroundView.size_hint());
		SortEntitiesByPivot(renderableGroundView, sortResult);
		DrawLayer(sortResult, window, renderableGroundView);
		sortResult.clear();
	}
}