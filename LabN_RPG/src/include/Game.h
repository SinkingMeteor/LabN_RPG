#pragma once

#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include <memory>
#include "World/World.h"
#include "Locator.h"
#include "World/GameWorld.h"
namespace vg 
{
	class Game 
	{
	public: 
		Game();
		void Update();

	private:
		sf::Clock m_clock;
		std::unique_ptr<World> m_currentWorld;

		void Start();
		void Tick(sf::Time deltaTime);
		void Render();

	};
}