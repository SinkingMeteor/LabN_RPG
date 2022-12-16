#pragma once
#include "entt/entt.hpp"
#include "SFML/Graphics.hpp"
namespace vg 
{
	class IRenderSystem 
	{
	public:
		virtual ~IRenderSystem() {};
		virtual void Render(entt::registry& registry, sf::RenderWindow& window) = 0;
	};

	class ISystem 
	{
	public:
		virtual ~ISystem() {};
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) = 0;
	};
}