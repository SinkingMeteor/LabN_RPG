#include "World/World.h"

namespace vg 
{
	World::World(Window* window) : 
		m_window(window),
		m_registry(),
		m_worldView()
	{
		m_worldView.setSize(window->GetScreenSize());
	}
}

