#pragma once
#include "SFML/Graphics.hpp"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace vg 
{
	class Window
	{
	public:
		Window();
		bool Update();
		sf::RenderWindow& GetWindow() { return m_window; }
		Keyboard& GetKeyboard() { return m_keyboard; }
		Mouse& GetMouse() { return m_mouse; }
	private:
		sf::RenderWindow m_window;
		Keyboard m_keyboard;
		Mouse m_mouse;

		void ProcessEvents();
	};
}