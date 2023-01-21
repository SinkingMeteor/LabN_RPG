#include "Systems/Gizmo/PivotDebugRenderSystem.h"
#include "Components/Components.h"
#include "Common/MathUtils.h"
namespace vg 
{
	void PivotDebugRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		auto pivotView = registry.view<TransformComponent>();

		for (entt::entity entity : pivotView)
		{
			TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
			sf::CircleShape circle{ 1.0f };
			circle.setPosition(transformComponent.GlobalTransform * VGMath::Zero);
			window.draw(circle);
		}
	}
}

