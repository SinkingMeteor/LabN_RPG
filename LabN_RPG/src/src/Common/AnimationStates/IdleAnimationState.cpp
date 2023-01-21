#include "AnimationStates/IdleAnimationState.h"

namespace vg 
{
	void IdleAnimationState::StartAnimation(entt::entity entity, entt::registry& m_registry)
	{
		if (!m_registry.all_of<AnimationComponent>(entity)) return;

		AnimationComponent& animationComponent = m_registry.get<AnimationComponent>(entity);
		entt::id_type animationId{ Database::AnimTypes::IDLE_S };

		if (m_registry.all_of<MovementComponent>(entity))
		{
			MovementComponent& movementComponent = m_registry.get<MovementComponent>(entity);
			animationId = VGUtils::GetIDByDirection("IDL", movementComponent.FacingDirection);
		}

		Animation& newAnimation = animationComponent.CurrentAnimationPack->Animations[animationId];

		if (&newAnimation == animationComponent.CurrentAnimation) return;

		animationComponent.CurrentAnimation = &newAnimation;
		animationComponent.CurrentFrame = 0.0f;
		animationComponent.CurrentIndex = 0;
	}
}