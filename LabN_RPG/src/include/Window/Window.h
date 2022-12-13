#pragma once
#include "SFML/Graphics.hpp"
#include "Locator.h"
namespace vg 
{
	class Window 
	{
	public:
		Window();
		bool Update();
		sf::RenderWindow& GetWindow() { return m_window; }
	private:
		sf::RenderWindow m_window;

		void InitializeServices();
		void ProcessEvents();
	};
}