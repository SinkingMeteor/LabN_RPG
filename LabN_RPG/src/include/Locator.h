#pragma once
#include "entt/entt.hpp"
#include "Window/Window.h"
namespace vg
{
	class Window;

	namespace Locator 
	{
		using ECS = entt::locator<entt::registry>;
		using GameWindow = entt::locator<Window>;
	}
}