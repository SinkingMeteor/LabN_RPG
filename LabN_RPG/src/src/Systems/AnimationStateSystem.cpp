#include "Systems/AnimationStateSystem.h"

namespace vg 
{
	AnimationStateSystem::AnimationStateSystem(World* world) :
		System(world),
		m_states()
	{
		m_states.reserve(2);
		m_states.push_back(std::make_unique<IdleAnimationState>());
		m_states.push_back(std::make_unique<MovementAnimationState>());
	}

	void AnimationStateSystem::Tick(entt::registry& registry, sf::Time deltaTime)
	{
		auto view = registry.view<SpriteComponent, AnimationComponent>();

		for (entt::entity entity : view)
		{
			AnimationComponent& animationComponent = view.get<AnimationComponent>(entity);
			std::size_t nextStateIndex = 0;
			unsigned char maxPriority = 0;

			for(std::size_t i = 0; i < m_states.size(); ++i)
			{
				unsigned char priority = m_states[i]->GetPriority();
				if (m_states[i]->IsConditionDone(entity, registry) && m_states[i]->GetPriority() > maxPriority)
				{
					nextStateIndex = i;
					maxPriority = priority;
				}
			}

			m_states[nextStateIndex]->StartAnimation(entity, registry);
		}
	}

}