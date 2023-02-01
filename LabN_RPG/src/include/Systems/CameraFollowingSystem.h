#pragma once
#include "vgpch.h"
#include "Systems/ISystem.h"
#include "World/World.h"
#include "Components/Components.h"
namespace vg 
{
	class CameraFollowingSystem final : public System 
	{
	public:
		CameraFollowingSystem(World* world) : System(world){}
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}