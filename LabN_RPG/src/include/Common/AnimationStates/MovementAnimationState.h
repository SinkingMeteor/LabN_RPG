#pragma once
#include "AnimationStates/IAnimationState.h"
#include "entt/entt.hpp"
#include "Components/Components.h"
#include "GameplayUtils.h"
#include "Database.h"
#include "MathUtils.h"
namespace vg
{
	class MovementAnimationState final : public IAnimationState
	{
	public:
		MovementAnimationState() {};
		virtual entt::id_type GetAnimationTypeId() const override { return Database::AnimTypes::G_MOVE; };
		virtual unsigned char GetPriority() const override { return 1; };
		virtual bool IsConditionDone(entt::entity entity, entt::registry& m_registry) const override;
		virtual void StartAnimation(entt::entity entity, entt::registry& m_registry) override;
	};
}