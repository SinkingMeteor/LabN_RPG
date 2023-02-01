#pragma once
#include "vgpch.h"
#include "Components/Components.h"
#include "Systems/ISystem.h"

namespace vg 
{
	class ApplyTransformSystem final : public System
	{
	public:
		ApplyTransformSystem(World* world) : System(world) {}
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	private:
		void ApplyTransformToChildren(entt::entity entity, entt::registry& registry, sf::Transform& parentTransform);
	};
}