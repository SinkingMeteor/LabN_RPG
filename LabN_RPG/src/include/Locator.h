#pragma once
#include "entt/entt.hpp"
#include "Window/Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
namespace vg
{
	class Window;

	namespace Locator 
	{
		using ECS = entt::locator<entt::registry>;
		using GameWindow = entt::locator<Window>;
		using Mouse = entt::locator<Mouse>;
		using Keyboard = entt::locator<Keyboard>;
	}
}