#include "Systems/ApplyTransformSystem.h"
#include <cassert>
namespace vg
{
	void ApplyTransformSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		entt::entity rootEntity = m_world->GetSceneRootEntity();
		sf::Transform rootTransform = sf::Transform::Identity;

		ApplyTransformToChildren(rootEntity, registry, rootTransform);
	}

	void ApplyTransformSystem::ApplyTransformToChildren(entt::entity rootEntity, entt::registry& registry, sf::Transform& parentTransform)
	{
		TransformComponent& transformComponent = registry.get<TransformComponent>(rootEntity);
		NodeComponent& nodeComponent = registry.get<NodeComponent>(rootEntity);

		transformComponent.GlobalTransform = parentTransform * transformComponent.LocalTransform;

		for (entt::entity entity : nodeComponent.Children)
		{
			assert(registry.all_of<NodeComponent>(entity) && registry.all_of<TransformComponent>(entity));

			ApplyTransformToChildren(entity, registry, transformComponent.GlobalTransform);
		}
	}

}