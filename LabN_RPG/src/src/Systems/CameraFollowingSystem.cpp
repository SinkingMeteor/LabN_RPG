#include "Systems/CameraFollowingSystem.h"

namespace vg 
{
	void CameraFollowingSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<TransformComponent, CameraTarget>();

		sf::View& worldView = m_world->GetWorldView();

		if (!view) return;

		for (entt::entity entity : view) 
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			worldView.setCenter(transformComponent.Position);
		}
	}
}