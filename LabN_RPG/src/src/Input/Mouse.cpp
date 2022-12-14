#include "Input/Mouse.h"

namespace vg 
{
	Mouse::MouseEvent::MouseEvent() :
		m_eventType(MouseEventType::None),
		m_LMBPressed(false),
		m_RMBPressed(false),
		m_x(0),
		m_y(0)
	{}

	Mouse::MouseEvent::MouseEvent(MouseEventType type, const Mouse& parent) :
		m_eventType(type),
		m_LMBPressed(parent.m_IsLMBPressed),
		m_RMBPressed(parent.m_IsRMBPressed),
		m_x(parent.m_x),
		m_y(parent.m_y)
	{}

	Mouse::MouseEvent Mouse::Read()
	{
		if (m_eventQueue.size() > 0u)
		{
			Mouse::MouseEvent mouseEvent = m_eventQueue.front();
			m_eventQueue.pop();
			return mouseEvent;
		}
		else
		{
			return Mouse::MouseEvent{};
		}
	}

	void Mouse::Flush()
	{
		m_eventQueue = std::queue<MouseEvent>();
	}


	void Mouse::OnMouseMove(int newX, int newY)
	{
		m_x = newX;
		m_y = newY;

		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::Move, *this));
		CropQueue();
	}

	void Mouse::OnLeftPressed()
	{
		m_IsLMBPressed = true;

		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::LMBPress, *this));
		CropQueue();
	}

	void Mouse::OnLeftReleased()
	{
		m_IsLMBPressed = false;

		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::LMBRelease, *this));
		CropQueue();
	}

	void Mouse::OnRightPressed()
	{
		m_IsRMBPressed = true;

		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::RMBPress, *this));
		CropQueue();
	}

	void Mouse::OnRightReleased()
	{
		m_IsRMBPressed = false;

		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::RMBRelease, *this));
		CropQueue();
	}

	void Mouse::OnWheel(float delta)
	{
		if (delta >= 0)
		{
			OnWheelUp();
		}
		else
		{
			OnWheelDown();
		}
	}

	void Mouse::OnWheelUp()
	{
		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::WheelUp, *this));
		CropQueue();
	}

	void Mouse::OnWheelDown()
	{
		m_eventQueue.push(Mouse::MouseEvent(MouseEventType::WheelDown, *this));
		CropQueue();
	}

	void Mouse::CropQueue()
	{
		while (m_eventQueue.size() > m_maxEventQueueSize)
		{
			m_eventQueue.pop();
		}
	}
}

