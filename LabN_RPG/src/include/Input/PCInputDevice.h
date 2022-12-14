#pragma once
#include "Input/IInputDevice.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

namespace vg 
{
	class PCInputDevice final : public IInputDevice
	{
	public:
		PCInputDevice();

	private:
		std::unique_ptr<Mouse> m_mouse;
		std::unique_ptr<Keyboard> m_keyboard;
	};
}