#include "World/World.h"
#include "Components/Components.h"
#include "MathUtils.h"
namespace vg 
{
	World::World(Window* window) : 
		m_window(window),
		m_registry(),
		m_rootEntity(m_registry.create())
	{
		m_registry.emplace<TransformComponent>(m_rootEntity);
		m_registry.emplace<NodeComponent>(m_rootEntity);
		m_registry.emplace<WorldPartitionComponent>(m_rootEntity);
	}
}

