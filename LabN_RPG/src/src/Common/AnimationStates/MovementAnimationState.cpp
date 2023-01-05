#include "AnimationStates/MovementAnimationState.h"

namespace vg
{
	bool MovementAnimationState::IsConditionDone(entt::entity entity, entt::registry& m_registry) const
	{
		if (!m_registry.all_of<MovementComponent>(entity)) return false;

		MovementComponent& movementComponent = m_registry.get<MovementComponent>(entity);
		return VGMath::GetLengthOfVector2(movementComponent.Velocity) > 0.0f;
	}

	void MovementAnimationState::StartAnimation(entt::entity entity, entt::registry& m_registry)
	{
		if (!m_registry.all_of<AnimationComponent, MovementComponent>(entity)) return;

		AnimationComponent& animationComponent = m_registry.get<AnimationComponent>(entity);
		MovementComponent& movementComponent = m_registry.get<MovementComponent>(entity);

		entt::id_type animationId = GameplayUtils::GetIDByDirection("MOV", movementComponent.Velocity);
		
		Animation& newAnimation = animationComponent.CurrentAnimationPack->Animations[animationId];

		if (&newAnimation == animationComponent.CurrentAnimation) return;

		animationComponent.CurrentAnimation = &newAnimation;
		animationComponent.CurrentFrame = 0.0f;
		animationComponent.CurrentIndex = 0;
	}

}