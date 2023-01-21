#pragma once
#include "Systems/ISystem.h"
namespace vg 
{
	class SpatialPartitionDebugRenderSystem final : public IRenderSystem 
	{
	public:
		SpatialPartitionDebugRenderSystem(World* world) : m_world(world) {};
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) override;
	private:
		World* m_world;
	};
}