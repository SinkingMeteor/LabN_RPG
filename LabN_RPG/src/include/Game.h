#pragma once

#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include "Locator.h"
namespace vg 
{
	class Game 
	{
	public: 
		Game();
		void Update();

	private:
		void Tick();
		void Render();

	};
}