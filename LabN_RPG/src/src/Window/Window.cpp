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

	bool Window::Update()
	{
		if (m_window.isOpen())
		{
			ProcessEvents();
			return true;
		}
		return false;
	}

	void Window::InitializeServices()
	{
		Locator::Window_t::emplace(m_window);
	}

	void Window::ProcessEvents()
	{
		sf::Event event{};
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_window.close();
			}
		}
	}

}

