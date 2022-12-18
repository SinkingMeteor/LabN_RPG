#pragma once
#include "Systems/ISystem.h"
#include "World/World.h"
#include "AnimationStates/IAnimationState.h"
#include "AnimationStates/IdleAnimationState.h"
#include "AnimationStates/MovementAnimationState.h"
namespace vg 
{
	class AnimationStateSystem final : public System 
	{
	public:
		AnimationStateSystem(World* world);
	private:
		std::vector<std::unique_ptr<IAnimationState>> m_states;
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}