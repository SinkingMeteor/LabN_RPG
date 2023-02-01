#pragma once
#include "vgpch.h"

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
		sf::View& GetView() { return m_view; }
		void SetView(const sf::View& view) { m_window.setView(view); }
		sf::Vector2f GetScreenSize() const { return sf::Vector2f{ 640.0f, 360.0f }; }
	private:
		sf::View m_view;
		sf::RenderWindow m_window;
		Keyboard m_keyboard;
		Mouse m_mouse;

		void ProcessEvents();
	};
}