#include "Systems/Gizmo/ColliderDebugRenderSystem.h"
#include "Components/Components.h"
#include "Common/MathUtils.h"
#include "SFML/Graphics.hpp"
namespace vg 
{
	void ColliderDebugRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		auto pivotView = registry.view<TransformComponent, RectColliderComponent>();

		for (entt::entity entity : pivotView)
		{
			TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
			RectColliderComponent& rectComponent = registry.get<RectColliderComponent>(entity);

			sf::RectangleShape rectangle{};
			rectangle.setFillColor(sf::Color::Transparent);
			
			if (registry.all_of<Trigger>(entity)) 
			{
				rectangle.setOutlineColor(sf::Color::Yellow);

				if (registry.all_of<Triggered>(entity)) 
				{
					rectangle.setOutlineColor(sf::Color::Red);
				}
			}
			else
			{
				rectangle.setOutlineColor(sf::Color::Green);
			}

			rectangle.setOutlineThickness(-1.0f);
			rectangle.setPosition(transformComponent.GlobalTransform * sf::Vector2f{ rectComponent.Rect.left, rectComponent.Rect.top });
			rectangle.setSize(sf::Vector2f{ rectComponent.Rect.width, rectComponent.Rect.height });
			window.draw(rectangle);
		}
	}
}