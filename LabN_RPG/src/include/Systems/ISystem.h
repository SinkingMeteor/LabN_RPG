#pragma once
#include "entt/entt.hpp"
#include "SFML/Graphics.hpp"
#include "World/World.h"
namespace vg 
{
	class IRenderSystem 
	{
	public:
		virtual ~IRenderSystem() {};
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) = 0;
	};

	class System 
	{
	public:
		System(World* world) : m_world(world) {};
		virtual ~System() {};
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) = 0;
	protected:
		World* m_world;
	};
}