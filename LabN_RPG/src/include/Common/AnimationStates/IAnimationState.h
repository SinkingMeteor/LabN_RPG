#pragma once
#include "entt/entt.hpp"

namespace vg
{
	class IAnimationState 
	{
	public:
		virtual ~IAnimationState() {};
		virtual unsigned char GetPriority() const = 0;
		virtual entt::id_type GetAnimationTypeId() const = 0;
		virtual bool IsConditionDone(entt::entity entity, entt::registry& m_registry) const = 0;
		virtual void StartAnimation(entt::entity entity, entt::registry& m_registry) = 0;
	};
}