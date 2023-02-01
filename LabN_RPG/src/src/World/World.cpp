#include "World/World.h"
#include "Components/Components.h"
#include "MathUtils.h"
namespace vg 
{
	World::World(Window* window) : 
		m_window(window),
		m_registry(),
		m_rootEntity(m_registry.create()),
		m_uniqueObjects()
	{
		m_registry.emplace<TransformComponent>(m_rootEntity);
		m_registry.emplace<NodeComponent>(m_rootEntity);
		m_registry.emplace<WorldPartitionComponent>(m_rootEntity);
	}

	void World::AddUniqueObject(entt::id_type id, entt::entity entity)
	{
		assert(!ContainsUniqueObject(id));
		m_uniqueObjects.emplace(id, entity);
	}

	void World::RemoveUniqueObject(entt::id_type id)
	{
		assert(ContainsUniqueObject(id));
		m_uniqueObjects.erase(id);
	}

	bool World::ContainsUniqueObject(entt::id_type id)
	{
		auto it = m_uniqueObjects.find(id);
		return it != m_uniqueObjects.end();
	}
}

