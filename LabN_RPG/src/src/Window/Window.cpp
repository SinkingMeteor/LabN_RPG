#include "Window/Window.h"

namespace vg 
{
	Window::Window() :
		m_view(),
		m_window(sf::VideoMode({ 640, 360 }), "Game"),
		m_keyboard(),
		m_mouse()
	{
		m_view.setSize(GetScreenSize());
		m_window.setFramerateLimit(60);
		m_window.setVerticalSyncEnabled(true);
		m_window.setKeyRepeatEnabled(false);
	}

	bool Window::Update()
	{
		if (m_window.isOpen())
		{
			ProcessEvents();
			return true;
		}
		return false;
	}

	void Window::ProcessEvents()
	{
		sf::Event event{};

		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::MouseMoved:
				m_mouse.OnMouseMove(event.mouseMove.x, event.mouseMove.y);
				break;

			case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					m_mouse.OnLeftPressed();
				else
					m_mouse.OnRightPressed();
			}
				break;
			case sf::Event::MouseButtonReleased: 
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					m_mouse.OnLeftReleased();
				else
					m_mouse.OnRightReleased();
			}
				break;
			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					m_mouse.OnWheel(event.mouseWheelScroll.delta);
				break;
			case sf::Event::MouseLeft:
				m_mouse.Flush();
				break;
			case sf::Event::LostFocus:
				m_mouse.Flush();
				break;
			case sf::Event::Resized:
				break;
			case sf::Event::KeyPressed:
				m_keyboard.OnKeyPressed(event.key.code);
				break;
			case sf::Event::KeyReleased:
				m_keyboard.OnKeyReleased(event.key.code);
				break;
			case sf::Event::Closed:
				m_window.close();
				break;
			default:
				break;
			}
		}
	}

}

