#pragma once
#include "SFML/Graphics.hpp"
#include <queue>

namespace vg 
{
	enum class MouseEventType
	{
		Move,
		WheelUp,
		WheelDown,
		LMBPress,
		LMBRelease,
		RMBPress,
		RMBRelease,
		None
	};

	class Mouse
	{
		class MouseEvent
		{
		public:
			MouseEvent();
			MouseEvent(MouseEventType type, const Mouse& parent);
			bool IsValid() const { return m_eventType != MouseEventType::None; }
			MouseEventType GetEventType() const { return m_eventType; }
			sf::Vector2i GetMouseLocation() const { return{ m_x,m_y }; }
			int GetPosX() const { return m_x; }
			int GetPosY() const { return m_y; }
			bool LeftIsPressed() const { return m_LMBPressed; }
			bool RightIsPressed() const { return m_RMBPressed; }

		private:
			MouseEventType m_eventType;
			bool m_LMBPressed;
			bool m_RMBPressed;
			int m_x;
			int m_y;
		};

		friend class Window;

	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;

		Mouse::MouseEvent Read();
		sf::Vector2i GetMousePosition() const { return sf::Vector2i{ m_x, m_y }; }
		bool IsLMBPressed() const { return m_IsLMBPressed; }
		bool IsRMBPressed() const { return m_IsRMBPressed; }
		int GetPositionX() const { return m_x; }
		int GetPositionY() const { return m_y; }
		bool IsNoEvents() const { return m_eventQueue.empty(); }
		void Flush();
	private:
		void OnMouseMove(int x, int y);
		void OnWheel(float delta);
		void OnWheelUp();
		void OnWheelDown();
		void OnLeftPressed();
		void OnLeftReleased();
		void OnRightPressed();
		void OnRightReleased();
		void CropQueue();
	private:
		static constexpr unsigned int m_maxEventQueueSize = 4u;
		int m_x;
		int m_y;
		bool m_IsLMBPressed = false;
		bool m_IsRMBPressed = false;
		std::queue<MouseEvent> m_eventQueue;
	};
}

