#pragma once
#include "entt/entt.hpp"
#include "Window/Window.h"
namespace vg
{
	namespace Locator 
	{
		using ECS_t = entt::locator<entt::registry>;
		using Window_t = entt::locator<Window>;
	}
}