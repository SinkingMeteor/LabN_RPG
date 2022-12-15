#include "Window/Window.h"

namespace vg 
{
	Window::Window() :
		m_window(sf::VideoMode(640, 360), "Game")
	{
		m_window.setFramerateLimit(60);
		m_window.setVerticalSyncEnabled(true);
		m_window.setKeyRepeatEnabled(false);

		InitializeServices();
	}

	void Window::InitializeServices() const
	{
		Locator::Keyboard::emplace();
		Locator::Mouse::emplace();
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

		Mouse& mouse = Locator::Mouse::value();
		Keyboard& keyboard = Locator::Keyboard::value();

		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::MouseMoved:
				mouse.OnMouseMove(event.mouseMove.x, event.mouseMove.y);
				break;

			case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					mouse.OnLeftPressed();
				else
					mouse.OnRightPressed();
			}
				break;
			case sf::Event::MouseButtonReleased: 
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					mouse.OnLeftReleased();
				else
					mouse.OnRightReleased();
			}
				break;
			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					mouse.OnWheel(event.mouseWheelScroll.delta);
				break;
			case sf::Event::MouseLeft:
				mouse.Flush();
				break;
			case sf::Event::LostFocus:
				mouse.Flush();
				break;
			case sf::Event::Resized:
				break;
			case sf::Event::KeyPressed:
				keyboard.OnKeyPressed(event.key.code);
				break;
			case sf::Event::KeyReleased:
				keyboard.OnKeyReleased(event.key.code);
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

