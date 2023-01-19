#include "SpacePartitioning/PartitionCell.h"
#include <cassert>
#include <algorithm>
namespace vg 
{
	using It = std::vector <entt::entity> ::iterator;

	PartitionCell::PartitionCell():
		m_entities()
	{}

	void PartitionCell::AddEntity(entt::entity entity)
	{
		m_entities.push_back(entity);
	}

	void PartitionCell::RemoveEntity(entt::entity entity)
	{
		It it = m_entities.begin();
		for (; it != m_entities.end(); ++it)
		{
			if (*it == entity)
				break;
		}

		m_entities.erase(it);
	}

	bool PartitionCell::Contains(entt::entity entity)
	{
		It it = m_entities.begin();
		for (; it != m_entities.end(); ++it)
		{
			if (*it == entity)
				return true;
		}
		return false;
	}
}

