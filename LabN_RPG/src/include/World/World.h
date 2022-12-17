#pragma once 
#include "SFML/Graphics.hpp"
#include "Window/Window.h"
#include "entt/entt.hpp"
namespace vg 
{
	class World 
	{
	public:
		World(Window* window) : m_window(window), m_registry() {}
		Window* GetWindow() { return m_window; }
		virtual ~World() {};
		virtual void Initialize() = 0;
		virtual void Tick(sf::Time deltaTime) = 0;
		virtual void Render() = 0;
	protected:
		Window* m_window;
		entt::registry m_registry;
	};
}