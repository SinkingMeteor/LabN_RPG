#pragma once
#include <queue>
#include <bitset>
#include <iostream>
#include "SFML/Graphics.hpp"

namespace vg 
{
	enum class KeyboardEventType
	{
		Pressed,
		Released,
		None
	};

	class Keyboard
	{
		class KeyboardEvent
		{
		public:
			KeyboardEvent();
			KeyboardEvent(KeyboardEventType type, sf::Keyboard::Key code);
			bool IsPress() const { return m_eventType == KeyboardEventType::Pressed; }
			bool IsRelease() const { return m_eventType == KeyboardEventType::Released; }
			bool IsValid() const { return m_eventType != KeyboardEventType::None; }
			sf::Keyboard::Key GetCode() const { return m_code; }
		private:
			KeyboardEventType m_eventType;
			sf::Keyboard::Key m_code;
		};

		friend class Window;

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		KeyboardEvent ReadKey();

		void Flush() { m_keyboardEventQueue = std::queue<KeyboardEvent>(); }
		bool IsNoKeyEvent() const { return m_keyboardEventQueue.empty(); }
		bool KeyIsPressed(sf::Keyboard::Key keycode) const { return m_keyStates[keycode]; }
	private:
		void OnKeyPressed(sf::Keyboard::Key keycode);
		void OnKeyReleased(sf::Keyboard::Key keycode);
		void CropQueue();
	private:
		static constexpr int m_maxEventQueueSize = 4u;
		std::bitset<sf::Keyboard::Key::KeyCount> m_keyStates;
		std::queue<KeyboardEvent> m_keyboardEventQueue;
	};
}

