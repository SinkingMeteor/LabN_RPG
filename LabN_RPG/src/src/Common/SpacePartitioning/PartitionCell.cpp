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
		//assert(std::find(m_entities.begin(), m_entities.end(), [&entity](entt::entity& checkEntity) { return entity == checkEntity; }) == m_entities.end());

		m_entities.push_back(entity);
	}

	void PartitionCell::RemoveEntity(entt::entity entity)
	{
		//It result = std::find(m_entities.begin(), m_entities.end(), [entity](entt::entity checkEntity) { return checkEntity == entity; });
		//if (result == m_entities.end()) return;
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

