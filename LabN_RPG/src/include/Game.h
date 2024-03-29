#pragma once
#include "vgpch.h"
#include "World/World.h"
#include "World/GameWorld.h"
namespace vg 
{
	class Game 
	{
	public: 
		Game();
		void Start();
		void Update();

	private:
		sf::Clock m_clock;
		std::unique_ptr<World> m_currentWorld;
		Window m_window;

		void Tick(sf::Time deltaTime);
		void Render();

	};
}