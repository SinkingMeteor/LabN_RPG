#pragma once
#include "SFML/Graphics.hpp"
#include "Locator.h"
namespace vg 
{
	class Window
	{
	public:
		Window();
		void InitializeServices() const;
		bool Update();
		sf::RenderWindow& GetWindow() { return m_window; }
	private:
		sf::RenderWindow m_window;

		void ProcessEvents();
	};
}