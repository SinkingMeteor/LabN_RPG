#include "Input/Keyboard.h"

namespace vg 
{
	Keyboard::KeyboardEvent Keyboard::ReadKey()
	{
		if (m_keyboardEventQueue.size() > 0u)
		{
			Keyboard::KeyboardEvent event = m_keyboardEventQueue.front();
			m_keyboardEventQueue.pop();
			return event;
		}
		else
		{
			return Keyboard::KeyboardEvent();
		}
	}

	void Keyboard::OnKeyPressed(sf::Keyboard::Key keycode)
	{
		m_keyStates[keycode] = true;
		m_keyboardEventQueue.push(Keyboard::KeyboardEvent(KeyboardEventType::Pressed, keycode));
		CropQueue();
	}

	void Keyboard::OnKeyReleased(sf::Keyboard::Key keycode)
	{
		m_keyStates[keycode] = false;
		m_keyboardEventQueue.push(Keyboard::KeyboardEvent(KeyboardEventType::Released, keycode));
		CropQueue();
	}

	void Keyboard::CropQueue()
	{
		while (m_keyboardEventQueue.size() > m_maxEventQueueSize)
		{
			m_keyboardEventQueue.pop();
		}
	}

	Keyboard::KeyboardEvent::KeyboardEvent():
	m_eventType(KeyboardEventType::None),
	m_code(sf::Keyboard::Key::Unknown)
	{}

	Keyboard::KeyboardEvent::KeyboardEvent(KeyboardEventType type, sf::Keyboard::Key code):
	m_eventType(type),
	m_code(code)
	{}
}
