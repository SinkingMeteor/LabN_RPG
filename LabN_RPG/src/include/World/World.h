#pragma once 
#include "SFML/Graphics.hpp"
#include "Window/Window.h"
#include "entt/entt.hpp"
namespace vg 
{
	class World 
	{
	public:
		World(Window* window);
		Window* GetWindow() { return m_window; }
		sf::View& GetWorldView() { return m_worldView; }
		virtual ~World() {};
		virtual void Initialize() = 0;
		virtual void Tick(sf::Time deltaTime) = 0;
		virtual void Render() = 0;
	protected:
		Window* m_window;
		entt::registry m_registry;
		sf::View m_worldView;
	};
}