#pragma once
#include "vgpch.h"
#include "AnimationStates/IAnimationState.h"
#include "Components/Components.h"
#include "GameplayUtils.h"
#include "Database.h"
namespace vg 
{
	class IdleAnimationState final : public IAnimationState 
	{
	public:
		IdleAnimationState() {};
		virtual entt::id_type GetAnimationTypeId() const override { return Database::AnimTypes::G_IDLE; };
		virtual unsigned char GetPriority() const override { return 0; };
		virtual bool IsConditionDone(entt::entity entity, entt::registry& m_registry) const override { return true; };
		virtual void StartAnimation(entt::entity entity, entt::registry& m_registry) override;
	};
}